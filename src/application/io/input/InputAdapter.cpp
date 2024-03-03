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

std::ostream &operator<<(std::ostream &s, binary_write_t<InputAdapter> inputAdapter) {
    auto &inp = inputAdapter.t;
    s << binw(inp.quitFlag);
    s << binw(inp.keys);
    return s;
}

std::istream &operator>>(std::istream &s, binary_read_t<InputAdapter> inputAdapter){
    auto &inp = inputAdapter.t;
    s >> binr(inp.quitFlag);
    s >> binr(inp.keys);
    return s;
}

