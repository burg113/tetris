//
// Created by Burg on 16.12.2023.
//

#include <SDL.h>
#include "SDLInputAdapter.h"

SDLInputAdapter::SDLInputAdapter() : InputAdapter(SDL_Scancode::SDL_NUM_SCANCODES){}


void SDLInputAdapter::update() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_KEYDOWN) {
            if (InputAdapter::isDown(event.key.keysym.scancode)) continue;
            InputAdapter::update(event.key.keysym.scancode, true);
        }else if (event.type == SDL_KEYUP) {
            InputAdapter::update(event.key.keysym.scancode, false);
        }else if (event.type == SDL_QUIT) {
            InputAdapter::doQuit();
        }
    }
}

bool SDLInputAdapter::isDown(int keyCode) {
    update();
    return InputAdapter::isDown(keyCode);
}
