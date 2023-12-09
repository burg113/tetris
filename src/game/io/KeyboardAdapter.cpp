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

            for (auto listener: listeners) listener->emplace(event.key.keysym.scancode);
        }
        if (event.type == SDL_KEYUP) {
            keys[event.key.keysym.scancode] = false;
            for (auto listener: listeners) listener->emplace(event.key.keysym.scancode);
        }
        if (event.type == SDL_QUIT) {
            quit = true;
            for (auto listener: listeners) listener->emplace(-1);
        }
    }

    for (auto listener: listeners) listener->update();

}

bool KeyboardAdapter::isDown(SDL_Scancode keyCode) {
    update();
    return keys[keyCode];
}

void KeyboardAdapter::registerListener(KeyboardAdapter::Listener *listener) {
    listeners.emplace(listener);
}

void KeyboardAdapter::unRegisterListener(KeyboardAdapter::Listener *listener) {
    listeners.erase(listener);
}

KeyboardAdapter::Listener::Listener(const std::vector<std::function<void()>> &callBacks,
                                    KeyboardAdapter *keyboardAdapter)
        : callBacks(callBacks), keyboardAdapter(keyboardAdapter) {

}

void KeyboardAdapter::Listener::emplace(int val) {
    eventStream.emplace_front(val);
}

void KeyboardAdapter::Listener::update() {
    for (const auto &f: callBacks) f();
}


bool KeyboardAdapter::Listener::hasEvent() {
    return !eventStream.empty();
}

int KeyboardAdapter::Listener::getEvent() {
    if (eventStream.empty()) return -1;
    int a = eventStream.back();
    eventStream.pop_back();
    return a;
}
