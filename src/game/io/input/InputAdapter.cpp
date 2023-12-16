//
// Created by Burg on 20.05.2023.
//

#include <iostream>
#include "InputAdapter.h"


InputAdapter::InputAdapter(int keySize) {
    keys = std::vector<bool>(keySize);
}

bool InputAdapter::isDown(int keyCode) {
    return keys[keyCode];
}

void InputAdapter::registerCallback(std::function<void(bool, int)> callback) {
    callbacks.emplace_back(callback);
}

void InputAdapter::registerExitCallback(std::function<void()> callback) {
    exitCallbacks.emplace_back(callback);
}

void InputAdapter::update(int key, bool state) {
    keys[key] = state;
    for (const auto &callback: callbacks) callback(state, key);
}

void InputAdapter::doQuit() {
    quitFlag = true;
    for (const auto &callback: exitCallbacks) callback();
}

bool InputAdapter::quit() {
    return quitFlag;
}

