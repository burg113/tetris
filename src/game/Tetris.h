//
// Created by Burg on 08.12.2023.
//

#ifndef TETRIS_TETRIS_H
#define TETRIS_TETRIS_H

#include "io/Window.h"
#include "io/KeyboardAdapter.h"
#include "Board.h"

class Tetris {
    Window *window;

    int myBoard;
    std::vector<Board> boards;

public:
    Tetris(Window *window) : window(window) {

    };

    void play();
};


#endif //TETRIS_TETRIS_H
