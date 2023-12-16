//
// Created by Burg on 09.12.2023.
//

#include "GameLogic.h"

#include <utility>

GameLogic::GameLogic() :board(0, 0), piece(iPiece){
    reset();
}


void GameLogic::setInputAdapter(InputAdapter* adapter) {
    inputAdapter = adapter;
    inputAdapter->registerCallback([this](bool b, int key) { this->handleInputCallback(b, key); });
}

void GameLogic::reset() {
    board = Board(10, 20);
    piece = iPiece;
    resetPiece();
}


void GameLogic::update() {
    handleInput();

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

    if (inputAdapter->isDown(Key::FAST_FALL)) {
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

    for (Vec2 v: piece.tiles) {
        Vec2 p = v.rot(rotation) + position;
        board[p] = 1;
    }

    board.update();

    resetPiece();

    if (!checkPos(position, piece, rotation)) {
        board.reset();
    }
}


void GameLogic::handleInput() {
    bool l = inputAdapter->isDown(Key::LEFT);
    bool r = inputAdapter->isDown(Key::RIGHT);
    if (l && !r) inpL++, inpR = -1;
    else if (r && !l) inpL = -1, inpR++;
    else inpL = inpR = -1;

    bool rl = inputAdapter->isDown(Key::ROTATE_LEFT);
    bool rr = inputAdapter->isDown(Key::ROTATE_RIGHT);
    if (rl && !rr) inpRl++, inpRr = -1;
    else if (rr && !rl) inpRl = -1, inpRr++;
    else inpRl = inpRr = -1;
}

void GameLogic::handleInputCallback(bool pressed, int key) {
    if (pressed && key == Key::INSTA_DROP) {
        instaDrop = true;
    }
}

void GameLogic::resetPiece() {
    piece = getNextTetromino();
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

    return (board[pos] == 0);
}

GameLogic::Piece GameLogic::getNextTetromino() {
    std::vector<Piece> pieces = {iPiece, jPiece, lPiece, oPiece, sPiece, tPiece, zPiece};
    return pieces[std::rand() % 7];
}

bool GameLogic::checkOffset(Vec2 posOffset) {
    return checkPos(position + posOffset, piece, rotation);
}

bool GameLogic::tryRotate(bool clockwise) {
    rotation = (rotation % 4 + 4) % 4;
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


GameLogic::Piece::Piece(std::vector<Vec2> tiles, int type, int colorId) : tiles(std::move(tiles)), type(type),
                                                                          colorId(colorId) {}
