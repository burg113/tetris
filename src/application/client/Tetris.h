//
// Created by Burg on 08.12.2023.
//

#pragma once

#include <stack>
#include <memory>
#include "application/io/output/sdl/Window.h"
#include "../io/input/InputAdapter.h"
#include "application/client/state/game/Board.h"
#include "state/ApplicationState.h"
#include "networking/SocketWrapper.h"

constexpr int STATE_COUNT = 3;
enum StateEnum {
    MAIN_MENU, GAME, MENU
};

class Tetris {
    int FRAMERATE = 60;
public:
    Window *window;
    InputAdapter *inputAdapter;

    SocketWrapper *socket;

    std::vector<std::unique_ptr<ApplicationState>> stateInstances;

    std::stack<StateEnum> state;


    Tetris(Window *window, InputAdapter *inputAdapter, SocketWrapper *socket);

    [[maybe_unused]] void popStateStack();
    void gotoState(StateEnum s);


    void reset();

    void play();

    void update();

};

