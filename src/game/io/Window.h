//
// Created by Burg on 08.12.2023.
//

#include <SDL.h>
#ifndef TETRIS_WINDOW_H
#define TETRIS_WINDOW_H


class Window {
    SDL_Window *window;

public:
    bool valid = false;
    Window();

    virtual ~Window();

};


#endif //TETRIS_WINDOW_H
