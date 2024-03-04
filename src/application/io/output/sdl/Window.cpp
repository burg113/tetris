//
// Created by Burg on 08.12.2023.
//
#include <bits/stdc++.h>
#include "Window.h"
// delay for closing
#define DELAY 100       // ms

Window::Window(int width, int height, const std::string &title) : width(width), height(height) {

    valid = false;
    /* Initialises data */
    window = NULL;

    /*
    * Initialises the SDL video subsystem (as well as the events subsystem).
    * Returns 0 on success or a negative error code on failure using SDL_GetError().
    */
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL failed to initialise: " << SDL_GetError() << "\n";
        return;
    }

    /* Creates a SDL window */
    window = SDL_CreateWindow(title.c_str(), /* Title of the SDL window */
                              SDL_WINDOWPOS_UNDEFINED, /* Position x of the window */
                              SDL_WINDOWPOS_UNDEFINED, /* Position y of the window */
                              width, /* Width of the window in pixels */
                              height, /* Height of the window in pixels */
                              0); /* Additional flag(s) */

    /* Checks if window has been created; if not, exits program */
    if (window == NULL) {
        std::cerr << "SDL window failed to initialise: " << SDL_GetError() << "\n";
        return;
    }
    valid = true;


    sdlRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void Window::render() {
    SDL_RenderClear(sdlRenderer);

    for(auto &[ind, obj] : renderObjects)
        SDL_RenderCopy(sdlRenderer, obj.texture, obj.srcRect, obj.dstRect);

    SDL_RenderPresent(sdlRenderer);
}



Window::~Window() {
    /* Pauses all SDL subsystems for a variable amount of milliseconds */
    SDL_Delay(DELAY);

    /* Frees memory */
    SDL_DestroyWindow(window);

    /* Shuts down all SDL subsystems */
    SDL_Quit();
}


int Window::registerTexture(SDL_Texture *texture,const SDL_Rect *srcRect, const SDL_Rect *dstRect) {
    if (renderObjects.count(IDCount)) std::cerr << "texture with index was already present and could not be added\n";
    renderObjects.insert({IDCount,RenderObject{texture, srcRect, dstRect}});
    return IDCount++;
}

void Window::removeTexture(int ind) {
    if (renderObjects.count(ind)) renderObjects.erase(ind);
    else
        std::cerr << "tried to remove texture with index " << ind << " that is not present.\n";
}

