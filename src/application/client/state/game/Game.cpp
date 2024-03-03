//
// Created by Burg on 16.12.2023.
//

#include "Game.h"
#include "application/io/input/SDLInputAdapter.h"
#include "networking/BinarySerialize.h"

Game::Game(Tetris *tetris) : tetris(tetris), inputData((int) GameLogic::Key::SIZE), gameLogic() {
    SDLInputAdapter::get()->registerCallback([this](bool set, int key) {
        if (KEY_CONVERSION.count(key)) {
            std::cerr << key << " " << set << std::endl;
            for (int k: KEY_CONVERSION[key]){
                inputData.update(k, set);
            }
        }
    });
}

void Game::sendFrameData() {
    std::stringstream strstr;
    strstr << binw((unsigned char) 0) << binw(gameLogic.frameCount) << binw(inputData);
    this->tetris->socket->send(strstr.str());
}

void Game::update() {
    SDLInputAdapter::get()->update();
    sendFrameData();
    gameLogic.update(inputData);
    inputData.update(GameLogic::Key::INSTA_DROP, false);
    render();
}

void Game::render() {
    Board *board = &gameLogic.board;

    float fill = .9;
    Window *window = tetris->window;
    int size = std::min(window->width / board->width, window->height / board->height) * fill;

    Vec2 upperCorner(window->width / 2 - size * board->width / 2,
                     window->height / 2 - size * board->height / 2);
    for (int y = 0; y < board->height; y++) {
        for (int x = 0; x < board->width; x++) {
            SDL_Color col;

            if ((*board)[x][y] == BOARD_INDEX_EMTPY) {
                col = {10, 10, 10, 255};
                window->draw(upperCorner.x + size * x, upperCorner.y + size * y, size, size, col);
            } else {
                col = colors[(*board)[x][y]];
                col.r *= .8;
                col.g *= .8;
                col.b *= .8;
                window->draw(upperCorner.x + size * x, upperCorner.y + size * y, size, size, col);

                col.r *= .6;
                col.g *= .6;
                col.b *= .6;
                window->drawBorder(upperCorner.x + size * x, upperCorner.y + size * y, size, size,
                                   boarderWidth,col);
            }
        }
    }

    Vec2 offset(0, size - (size * gameLogic.framesToFall) / gameLogic.framesPerFall);
    if (!gameLogic.checkOffset(Vec2(0, 1)) || !smoothAnimation) offset = Vec2(0, 0);

    for (Vec2 v: gameLogic.pieces[gameLogic.pieceInd].tiles) {
        Vec2 p = upperCorner + offset + (v.rot(gameLogic.rotation) + gameLogic.position) * size;
        window->draw(p.x, p.y, size, size, colors[gameLogic.pieces[gameLogic.pieceInd].colorId]);
    }
}
