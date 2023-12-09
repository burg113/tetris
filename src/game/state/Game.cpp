//
// Created by Burg on 09.12.2023.
//

#include "Game.h"

#include <utility>

void Game::update() {
    bool l = keyboard->isDown(SDL_Scancode::SDL_SCANCODE_LEFT);
    bool r = keyboard->isDown(SDL_Scancode::SDL_SCANCODE_RIGHT);
    if (l && !r) inpL++, inpR = 0;
    else if (r && !l) inpL = 0, inpR++;
    else inpL = inpR = 0;

    bool rl = keyboard->isDown(SDL_Scancode::SDL_SCANCODE_DOWN);
    bool rr = keyboard->isDown(SDL_Scancode::SDL_SCANCODE_UP);

    if (rl && !rr) inpRl++, inpRr = 0;
    else if (rr && !rl) inpRl = 0, inpRr++;
    else inpRl = inpRr = 0;


    if (inpL % holdFrameCooldown == 1 && checkOffset(Vec2(-1, 0))) position.x -= 1;
    if (inpR % holdFrameCooldown == 1 && checkOffset(Vec2(1, 0))) position.x += 1;

    if (inpRl % holdFrameRotationCooldown == 1) tryRotate(0);
    if (inpRr % holdFrameRotationCooldown == 1) tryRotate(1);


    if (framesToFall <= 0) {
        if (!checkOffset(Vec2(0, 1))) {
            if (-framesToFall >= lockLeniency) {
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
        } else {
            framesToFall = framesPerFall;
            position += Vec2(0, 1);
        }
    }

    frameCount++;
    framesToFall--;
    render();
}

void Game::render() {
    float fill = .9;
    Window *window = tetris->window;
    int size = std::min(window->width / board.width, window->height / board.height) * fill;

    Vec2 upperCorner(window->width / 2 - size * board.width / 2,
                     window->height / 2 - size * board.height / 2);
    for (int y = 0; y < board.height; y++) {
        for (int x = 0; x < board.width; x++) {
            SDL_Color col;

            if (board[x][y] == 0)
                col = {(unsigned char) (x * 10 + 50), (unsigned char) (y * 5 + 50), 0, 255};
            else
                col = {200, 200, 200, 255};

            window->draw(upperCorner.x + size * x, upperCorner.y + size * y, size, size, col);
        }
    }
    Vec2 offset(0, size - (size * framesToFall) / framesPerFall);
    if (!checkOffset(Vec2(0, 1)) || !smoothAnimation) offset = Vec2(0, 0);

    for (Vec2 v: piece.tiles) {
        Vec2 p = upperCorner + offset + (v.rot(rotation) + position) * size;
        window->draw(p.x, p.y, size, size, piece.col);
    }
}


void Game::reset() {
    board = Board(10, 20);
    piece = iPiece;
    resetPiece();
}

void Game::resetPiece() {
    piece = getNextTetromino();

    position = Vec2((board.width - 1) / 2, 1);
    rotation = 0;
}


Game::Game(Tetris *tetris) : tetris(tetris), board(0, 0), piece(iPiece) {
    keyboard = KeyboardAdapter::get();
    reset();
}

bool Game::checkPos(Vec2 pos, Piece &p, int rot) {
    for (Vec2 v: p.tiles) {
        if (!checkPos(pos + v.rot(rot)))
            return false;
    }
    return true;
}

bool Game::checkPos(Vec2 pos) {
    if (pos.x < 0 || pos.y < 0 || pos.x >= board.width || pos.y >= board.height)
        return false;

    return (board[pos] == 0);
}

Game::Piece Game::getNextTetromino() {
    std::vector<Piece> pieces = {iPiece, jPiece, lPiece, oPiece, sPiece, tPiece, zPiece};
    return pieces[std::rand() % 7];
}

bool Game::checkOffset(Vec2 posOffset) {
    return checkPos(position + posOffset, piece, rotation);
}

void Game::tryRotate(bool clockwise) {
    rotation = (rotation % 4 + 4) % 4;
    if (clockwise) {
        for (int i = 0; i < rotationTable[piece.type][0].size(); i++) {
            Vec2 rotOffset = rotationTable[piece.type][rotation][i] - rotationTable[piece.type][(rotation+1)%4][i];
            if (checkPos(position + rotOffset, piece, rotation + 1)) {
                position += rotOffset;
                rotation++;
                break;
            }
        }
    }
}

Game::Piece::Piece(std::vector<Vec2> tiles, int type, SDL_Color col) : tiles(std::move(tiles)), type(type), col(col) {}
