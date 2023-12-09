//
// Created by Burg on 08.12.2023.
//

#ifndef TETRIS_TETRIS_H
#define TETRIS_TETRIS_H

#include <stack>
#include <memory>
#include "io/Window.h"
#include "io/KeyboardAdapter.h"
#include "state/Board.h"
#include "state/State.h"

constexpr int STATE_COUNT = 3;
enum StateEnum {
    MAIN_MENU, GAME, MENU
};

class Tetris {
    int FRAMERATE = 60;
public:
    Window *window;
    KeyboardAdapter *keyboard;

    std::vector<std::unique_ptr<State>> stateInstances;

    std::stack<StateEnum> state;


    Tetris(Window *window);

    void popStateStack();
    void gotoState(StateEnum s);


    void reset();

    void play();

    void update();

};


#endif //TETRIS_TETRIS_H
