//
// Created by Burg on 20.05.2023.
//

#ifndef NON_EUCLIDEAN_RENDER_KEYBOARDADAPTER_H
#define NON_EUCLIDEAN_RENDER_KEYBOARDADAPTER_H

#include <SDL.h>
#include <map>
#include <vector>
#include <deque>
#include <set>
#include <functional>

// should be unique
class KeyboardAdapter {
public:


private:
    std::vector<bool> keys;

    std::vector<std::function<void(bool,int)>> callbacks;
    std::vector<std::function<void()>> exitCallbacks;

    KeyboardAdapter() {
        keys = std::vector<bool>(SDL_Scancode::SDL_NUM_SCANCODES);
    };

public:
    KeyboardAdapter(KeyboardAdapter const&) = delete;
    void operator=(KeyboardAdapter const&)  = delete;

    bool quit = false;

    static KeyboardAdapter* get(){
        static KeyboardAdapter keyboardAdapter;
        return &keyboardAdapter;
    }

    // updates the state
    void update();

    // callbacks will be called when a keypress/ depress is detected
    void registerCallback(std::function<void(bool,int)> callbacks);

    // will be called if exit request was sent
    void registerExitCallback(std::function<void()> callbacks);

    // updates the state and returns true if the given key is down
    bool isDown(SDL_Scancode keyCode);

};


#endif //NON_EUCLIDEAN_RENDER_KEYBOARDADAPTER_H
