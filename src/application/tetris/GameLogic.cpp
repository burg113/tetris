//
// Created by Burg on 09.12.2023.
//

#include "GameLogic.h"

#include <utility>

GameLogic::GameLogic() : board(0, 0), pieceInd(5) {
    reset();
}

void GameLogic::reset() {
    // TODO:
    std::srand(42);
    board = Board(10, 20);
    pieceInd = 5;
    resetPiece();
}


void GameLogic::update(const InputData &inputData) {
    handleInput(inputData);

    if (inpL >= 0 && checkOffset(Vec2(-1, 0))) {
        inpL = -holdFrameCooldown;
        position.x -= 1;
    }
    if (inpR >= 0 && checkOffset(Vec2(1, 0))) {
        inpR = -holdFrameCooldown;
        position.x += 1;
    }

    if (inpRl % holdFrameRotationCooldown >= 0) {
        if (tryRotate(false)) inpRl = -holdFrameRotationCooldown;
    }
    if (inpRr >= 0) {
        if (tryRotate(true)) inpRr = -holdFrameRotationCooldown;
    }

    if (inputData.isDown(Key::FAST_FALL)) {
        framesToFall -= fastFallExtraSpeed;
    }

    if (instaDrop) {
        while (checkOffset(Vec2(0, 1))) position += Vec2(0, 1);
        lockPiece();
        instaDrop = false;
    }


    if (framesToFall <= 0) {
        if (!checkOffset(Vec2(0, 1))) {
            if (-framesToFall >= lockLeniency) {
                lockPiece();
            }
        } else {
            framesToFall = framesPerFall;
            position += Vec2(0, 1);
        }
    }

    frameCount++;
    framesToFall--;
}

void GameLogic::lockPiece() {
    framesToFall = framesPerFall;

    for (Vec2 v: pieces[pieceInd].tiles) {
        Vec2 p = v.rot(rotation) + position;
        board[p] = pieces[pieceInd].colorId;
    }

    board.update();

    resetPiece();

    if (!checkPos(position, pieces[pieceInd], rotation)) {
        board.reset();
    }
}


void GameLogic::handleInput(const InputData &inputData) {
    bool l = inputData.isDown(Key::LEFT);
    bool r = inputData.isDown(Key::RIGHT);
    if (l && !r) inpL++, inpR = -1;
    else if (r && !l) inpL = -1, inpR++;
    else inpL = inpR = -1;

    bool rl = inputData.isDown(Key::ROTATE_LEFT);
    bool rr = inputData.isDown(Key::ROTATE_RIGHT);
    if (rl && !rr) inpRl++, inpRr = -1;
    else if (rr && !rl) inpRl = -1, inpRr++;
    else inpRl = inpRr = -1;

    if (inputData.isDown(Key::INSTA_DROP)) {
        instaDrop = true;
    }
}

void GameLogic::resetPiece() {
    pieceInd = getNextTetromino();
    position = Vec2((board.width - 1) / 2, 1);
    rotation = 0;
}

bool GameLogic::checkPos(Vec2 pos, Piece &p, int rot) {
    for (Vec2 v: p.tiles) {
        if (!checkPos(pos + v.rot(rot)))
            return false;
    }
    return true;
}

bool GameLogic::checkPos(Vec2 pos) {
    if (pos.x < 0 || pos.y < 0 || pos.x >= board.width || pos.y >= board.height)
        return false;

    return (board[pos] == BOARD_INDEX_EMTPY);
}

int GameLogic::getNextTetromino() {
    return frameCount % 7; // todo
}

bool GameLogic::checkOffset(Vec2 posOffset) {
    return checkPos(position + posOffset, pieces[pieceInd], rotation);
}

bool GameLogic::tryRotate(bool clockwise) {
    rotation = (rotation % 4 + 4) % 4;
    Piece &piece = pieces[pieceInd];
    if (clockwise) {
        for (int i = 0; i < rotationTable[piece.type][0].size(); i++) {
            Vec2 rotOffset = rotationTable[piece.type][rotation][i] - rotationTable[piece.type][(rotation + 1) % 4][i];
            if (checkPos(position + rotOffset, piece, rotation + 1)) {
                position += rotOffset;
                rotation++;
                return true;
            }
        }
    } else {
        for (int i = 0; i < rotationTable[piece.type][0].size(); i++) {
            Vec2 rotOffset = rotationTable[piece.type][rotation][i] - rotationTable[piece.type][(rotation + 3) % 4][i];
            if (checkPos(position + rotOffset, piece, rotation + 1)) {
                position += rotOffset;
                rotation--;
                return true;
            }
        }
    }
    return false;
}

BinaryStream &operator<<(BinaryStream &s, const GameLogic& gameLogic) {
    const auto &g = gameLogic;
    s << g.board << g.inpL << g.inpR << g.inpRl << g.inpRr
      << g.frameCount << g.framesToFall << g.position << g.rotation << g.pieceInd;
    return s;
}

BinaryStream &operator>>(BinaryStream &s, GameLogic& gameLogic) {
    auto &g = gameLogic;
    s >> g.board >> g.inpL >> g.inpR >> g.inpRl >> g.inpRr
      >> g.frameCount >> g.framesToFall >> g.position >> g.rotation >> g.pieceInd;
    return s;
}


GameLogic::Piece::Piece(std::vector<Vec2> tiles, int type, int colorId) : tiles(std::move(tiles)), type(type),
                                                                          colorId(colorId) {}
