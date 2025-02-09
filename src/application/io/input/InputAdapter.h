//
// Created by Burg on 20.05.2023.
//

#pragma once

#include <map>
#include <vector>
#include <deque>
#include <set>
#include <functional>
#include "networking/BinarySerialize.h"


// should be unique
class InputAdapter {
    std::vector<std::function<void(bool, int)>> callbacks;
    std::vector<std::function<void()>> exitCallbacks;
    std::vector<bool> keys;
    bool quitFlag = false;

protected:

    virtual void update(int key, bool state);

    void doQuit();

public:
    explicit InputAdapter(int keySize);

    // returns true if the given key is down
    virtual bool isDown(int keyCode);

    // callbacks will be called when a keypress/ depress is detected
    void registerCallback(const std::function<void(bool, int)>& callbacks);

    // will be called if exit request was sent
    [[maybe_unused]] void registerExitCallback(const std::function<void()>& callbacks);

    bool quit();

};

