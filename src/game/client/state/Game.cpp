//
// Created by Burg on 16.12.2023.
//

#include "Game.h"
#include "game/io/input/SDLInputAdapter.h"
#include "networking/BinarySerialize.h"

Game::Game(Tetris *tetris) : tetris(tetris),virtualAdapter((int) GameLogic::Key::SIZE), gameLogic() {
    gameLogic.setInputAdapter(&virtualAdapter);
    SDLInputAdapter::get() -> registerCallback([this](bool set, int key) {
        if (KEY_CONVERSION.count(key)) {
            for(int k : KEY_CONVERSION[key]) virtualAdapter.update(k,set);
        }
    });
    virtualAdapter.registerCallback([this](bool set, int key){
        std::stringstream strstr;
        strstr << binw((unsigned char) 0) << binw(key) << binw(set);
        this->tetris->socket->send(strstr.str());
    });
}

void Game::update() {
    SDLInputAdapter::get()->update();

    std::stringstream strstr;
    strstr << binw((unsigned char) 1) << gameLogic.frameCount;
    this->tetris->socket->send(strstr.str());

    gameLogic.update();
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

            if ((*board)[x][y] == 0)
                col = {(unsigned char) (x * 10 + 50), (unsigned char) (y * 5 + 50), 0, 255};
            else
                col = {200, 200, 200, 255};

            window->draw(upperCorner.x + size * x, upperCorner.y + size * y, size, size, col);
        }
    }

    Vec2 offset(0, size - (size * gameLogic.framesToFall) / gameLogic.framesPerFall);
    if (!gameLogic.checkOffset(Vec2(0, 1)) || !smoothAnimation) offset = Vec2(0, 0);

    for (Vec2 v: gameLogic.piece.tiles) {
        Vec2 p = upperCorner + offset + (v.rot(gameLogic.rotation) + gameLogic.position) * size;
        window->draw(p.x, p.y, size, size, colors[gameLogic.piece.colorId]);
    }
}
