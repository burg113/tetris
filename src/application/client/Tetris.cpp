//
// Created by Burg on 08.12.2023.
//

#include <chrono>
#include <thread>
#include <iostream>
#include "spdlog/spdlog.h"
#include "Tetris.h"

#include "state/ApplicationState.h"
#include "application/client/state/main_menu/MainMenu.h"
#include "application/client/state/game/Game.h"
#include "application/client/state/menu/Menu.h"

void Tetris::play() {
    auto t1 = std::chrono::high_resolution_clock::now();
    int frameCount = 0;

    while (!inputAdapter->quit()) {
        while (std::chrono::duration_cast<std::chrono::microseconds>(
                t1 - std::chrono::high_resolution_clock::now()).count() + (int) 1e6 / FRAMERATE> 0)
            continue;

        SPDLOG_TRACE("Frame took: {} ms", (int) std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::high_resolution_clock::now() - t1).count());

        t1 = std::chrono::high_resolution_clock::now();
        frameCount++;

        // todo: update input adapter!

        update();

        window->render();


    }

}

void Tetris::update() {
    if (state.empty()) {
        SPDLOG_CRITICAL("Fatal Error! GameLogic has lost state. Restarting.");
        reset();
        return;
    }
    stateInstances[state.top()]->update();
}


void Tetris::reset() {
    stateInstances = std::vector<std::unique_ptr<ApplicationState>>(STATE_COUNT);
    stateInstances[StateEnum::MAIN_MENU] = std::unique_ptr<ApplicationState>(new MainMenu(this));
    stateInstances[StateEnum::GAME] = std::unique_ptr<ApplicationState>(new Game(this));
    stateInstances[StateEnum::MENU] = std::unique_ptr<ApplicationState>(new Menu(this));

    state = std::stack<StateEnum>();
    state.emplace(MAIN_MENU);
}

Tetris::Tetris(Window *window, InputAdapter *inputAdapter, SocketWrapper *socket)
        : window(window),inputAdapter(inputAdapter), socket(socket) {
    reset();
}

void Tetris::gotoState(StateEnum s) {
    state.emplace(s);
}

[[maybe_unused]] void Tetris::popStateStack() {
    if (state.empty()) {
        SPDLOG_WARN("Cannot pop state. ApplicationState stack is already empty!");
        return;
    }
    state.pop();
}

