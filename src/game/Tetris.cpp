//
// Created by Burg on 08.12.2023.
//

#include <chrono>
#include <thread>
#include <iostream>
#include "Tetris.h"

#include "state/State.h"
#include "state/MainMenu.h"
#include "state/Game.h"
#include "state/Menu.h"

void Tetris::play() {
    auto t1 = std::chrono::high_resolution_clock::now();
    int frameCount = 0;

    while (!inputAdapter->quit()) {
        int microTillNextFrame = (int) std::chrono::duration_cast<std::chrono::microseconds>(
                t1 - std::chrono::high_resolution_clock::now()).count() + (int) 1e6 / FRAMERATE;
        if (microTillNextFrame > 0)
            std::this_thread::sleep_for(std::chrono::microseconds(microTillNextFrame));

        t1 = std::chrono::high_resolution_clock::now();
        frameCount++;

        // todo: update input adapter!

        update();

        window->render();

        /*std::cerr << "Frame took: " << (int) std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - t1).count() << " ms"<<std::endl;
        */
    }

}

void Tetris::update() {
    if (state.empty()) {
        std::cerr << "Fatal Error! Game has lost state. Restarting.";
        reset();
        return;
    }
    stateInstances[state.top()]->update();
}


void Tetris::reset() {
    stateInstances = std::vector<std::unique_ptr<State>>(STATE_COUNT);
    stateInstances[StateEnum::MAIN_MENU] = std::unique_ptr<State>(new MainMenu(this));
    stateInstances[StateEnum::GAME] = std::unique_ptr<State>(new Game(this,inputAdapter));
    stateInstances[StateEnum::MENU] = std::unique_ptr<State>(new Menu(this));

    state = std::stack<StateEnum>();
    state.emplace(MAIN_MENU);
}

Tetris::Tetris(Window *window, InputAdapter *inputAdapter) : window(window),inputAdapter(inputAdapter) {
    reset();
}

void Tetris::gotoState(StateEnum s) {
    state.emplace(s);
}

void Tetris::popStateStack() {
    if (state.empty()) {
        std::cerr << "Cannot pop state. State stack is already empty!"<<std::endl;
        return;
    }
    state.pop();
}

