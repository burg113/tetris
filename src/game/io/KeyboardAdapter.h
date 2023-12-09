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
    class Listener {
        std::deque<std::pair<bool,int>> eventStream;                // oldest events can be read& popped from back
        std::vector<std::function<void()>> callBacks;
    public:
        explicit Listener(const std::vector<std::function<void()>> &callBacks);

        void emplace(bool b, int val);
        void update();

        bool hasEvent();
        std::pair<bool,int> extractEvent();     // gets next event. -1 if no events are present. Note that -1 may also be a valid event.
        void voidEvents();
    };
private:
    std::vector<bool> keys;

    std::set<Listener *> listeners;
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

    // listeners will
    void registerListener(Listener *listener);

    void unRegisterListener(Listener *listener);

    // updates the state and returns true if the given key is down
    bool isDown(SDL_Scancode keyCode);

};


#endif //NON_EUCLIDEAN_RENDER_KEYBOARDADAPTER_H
