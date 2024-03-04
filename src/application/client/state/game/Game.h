//
// Created by Burg on 16.12.2023.
//

#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include "application/client/state/ApplicationState.h"
#include "application/client/Tetris.h"
#include "application/io/input/InputAdapter.h"
#include "application/tetris/GameLogic.h"
#include "application/io/input/VirtualInputAdapter.h"

class Game : public ApplicationState{
    GameLogic gameLogic;

    Tetris *tetris;
    InputData inputData;

    std::deque<std::pair<int,InputData>> inputHistory;

    SDL_Texture *texture;

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


    void sendFrameData();

public:
    explicit Game(Tetris *tetris);

    void update() override;

    void render();

};


#endif //TETRIS_GAME_H
