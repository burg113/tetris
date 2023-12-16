//
// Created by Burg on 08.12.2023.
//

#ifndef TETRIS_TETRIS_H
#define TETRIS_TETRIS_H

#include <stack>
#include <memory>
#include "../io/Window.h"
#include "../io/input/InputAdapter.h"
#include "state/Board.h"
#include "state/ApplicationState.h"

constexpr int STATE_COUNT = 3;
enum StateEnum {
    MAIN_MENU, GAME, MENU
};

class Tetris {
    int FRAMERATE = 60;
public:
    Window *window;
    InputAdapter *inputAdapter;

    std::vector<std::unique_ptr<ApplicationState>> stateInstances;

    std::stack<StateEnum> state;


    Tetris(Window *window, InputAdapter *inputAdapter);

    void popStateStack();
    void gotoState(StateEnum s);


    void reset();

    void play();

    void update();

};


#endif //TETRIS_TETRIS_H
