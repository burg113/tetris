//
// Created by Burg on 16.12.2023.
//

#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include "ApplicationState.h"
#include "../Tetris.h"
#include "game/io/input/InputAdapter.h"
#include "game/GameLogic.h"
#include "game/io/input/VirtualInputAdapter.h"

class Game : public ApplicationState{
    GameLogic gameLogic;
    bool smoothAnimation = true;

    Tetris *tetris;

    VirtualInputAdapter virtualAdapter;

    SDL_Scancode KEY_INSTA_DROP = SDL_Scancode::SDL_SCANCODE_SPACE;
    SDL_Scancode KEY_FAST_FALL = SDL_Scancode::SDL_SCANCODE_LCTRL;
    SDL_Scancode KEY_RIGHT = SDL_Scancode::SDL_SCANCODE_RIGHT;
    SDL_Scancode KEY_LEFT = SDL_Scancode::SDL_SCANCODE_LEFT;
    SDL_Scancode KEY_ROTATE_RIGHT = SDL_Scancode::SDL_SCANCODE_UP;
    SDL_Scancode KEY_ROTATE_LEFT = SDL_Scancode::SDL_SCANCODE_DOWN;

    std::map<int,std::vector<int>> KEY_CONVERSION = {
            {KEY_LEFT,{GameLogic::Key::LEFT}},
            {KEY_RIGHT,{GameLogic::Key::RIGHT}},
            {KEY_ROTATE_LEFT,{GameLogic::Key::ROTATE_LEFT}},
            {KEY_ROTATE_RIGHT,{GameLogic::Key::ROTATE_RIGHT}},
            {KEY_FAST_FALL,{GameLogic::Key::FAST_FALL}},
            {KEY_INSTA_DROP,{GameLogic::Key::INSTA_DROP}},

    };

    std::vector<SDL_Color> colors = {{20, 20, 200, 255},
                                     {240, 180, 20, 255},
                                     {15, 230, 15, 255},
                                     {140, 20, 180, 255},
                                     {200, 20, 20, 255},
                                     {20, 200, 240, 255},
                                     {215, 220, 0, 255}};

    int boarderWidth = 3;


public:
    explicit Game(Tetris *tetris);

    void update() override;

    void render();

};


#endif //TETRIS_GAME_H
