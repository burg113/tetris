//
// Created by Burg on 20.05.2023.
//

#ifndef NON_EUCLIDEAN_RENDER_KEYBOARDADAPTER_H
#define NON_EUCLIDEAN_RENDER_KEYBOARDADAPTER_H


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
    void registerCallback(std::function<void(bool, int)> callbacks);

    // will be called if exit request was sent
    void registerExitCallback(std::function<void()> callbacks);

    bool quit();

    friend std::ostream& operator << (std::ostream &s, binary_write_t<InputAdapter> inputAdapter);
    friend std::istream& operator >> (std::istream &s, binary_read_t<InputAdapter> inputAdapter);
};


#endif //NON_EUCLIDEAN_RENDER_KEYBOARDADAPTER_H
