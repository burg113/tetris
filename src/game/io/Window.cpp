//
// Created by Burg on 08.12.2023.
//
#include <bits/stdc++.h>
#include "Window.h"

/* Sets constants */
#define WIDTH 800
#define HEIGHT 600
#define DELAY 500       // ms

Window::Window() {
    valid = false;
    /* Initialises data */
    window = NULL;

    /*
    * Initialises the SDL video subsystem (as well as the events subsystem).
    * Returns 0 on success or a negative error code on failure using SDL_GetError().
    */
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr <<  "SDL failed to initialise: " <<  SDL_GetError()<<"\n";
        return;
    }

    /* Creates a SDL window */
    window = SDL_CreateWindow("Tetris", /* Title of the SDL window */
                              SDL_WINDOWPOS_UNDEFINED, /* Position x of the window */
                              SDL_WINDOWPOS_UNDEFINED, /* Position y of the window */
                              WIDTH, /* Width of the window in pixels */
                              HEIGHT, /* Height of the window in pixels */
                              0); /* Additional flag(s) */

    /* Checks if window has been created; if not, exits program */
    if (window == NULL) {
        std::cerr <<  "SDL window failed to initialise: "<< SDL_GetError() << "\n";
        return;
    }
    valid = true;
}


Window::~Window() {
    /* Pauses all SDL subsystems for a variable amount of milliseconds */
    SDL_Delay(DELAY);

    /* Frees memory */
    SDL_DestroyWindow(window);

    /* Shuts down all SDL subsystems */
    SDL_Quit();

}

