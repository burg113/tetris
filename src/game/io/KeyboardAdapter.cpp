//
// Created by Burg on 20.05.2023.
//

#include <iostream>
#include "KeyboardAdapter.h"

void KeyboardAdapter::update() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            keys[event.key.keysym.scancode] = true;

            for (const auto& callback: callbacks) callback(true,event.key.keysym.scancode);
        }else if (event.type == SDL_KEYUP) {
            keys[event.key.keysym.scancode] = false;
            for (const auto& callback: callbacks) callback(false,event.key.keysym.scancode);
        }
        if (event.type == SDL_QUIT) {
            quit = true;
            for (const auto& callback: exitCallbacks) callback();
        }
    }
}

bool KeyboardAdapter::isDown(SDL_Scancode keyCode) {
    update();
    return keys[keyCode];
}

void KeyboardAdapter::registerCallback(std::function<void(bool, int)> callback) {
    callbacks.emplace_back(callback);
}

void KeyboardAdapter::registerExitCallback(std::function<void()> callback) {
    exitCallbacks.emplace_back(callback);
}

