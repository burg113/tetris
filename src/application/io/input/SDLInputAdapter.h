//
// Created by Burg on 16.12.2023.
//

#pragma once

#include "InputAdapter.h"

class SDLInputAdapter : public InputAdapter{
private:

    SDLInputAdapter();

public:
    SDLInputAdapter(SDLInputAdapter const&) = delete;
    void operator=(SDLInputAdapter const&)  = delete;

    static SDLInputAdapter* get(){
        static SDLInputAdapter inputAdapter;
        return &inputAdapter;
    }

    // updates the state
    void update();

    // updates the state and returns true if the given key is down
    bool isDown(int keyCode) override;

};

