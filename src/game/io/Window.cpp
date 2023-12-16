//
// Created by Burg on 08.12.2023.
//
#include <bits/stdc++.h>
#include "Window.h"
// delay for closing
#define DELAY 100       // ms

Window::Window(int width, int height, const std::string &title) : width(width), height(height) {
    data = std::vector<uint8_t>(width * height * 4, 0);

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
    buffer = SDL_CreateTexture(sdlRenderer,
                               SDL_PIXELFORMAT_RGBA8888,
                               SDL_TEXTUREACCESS_STREAMING,
                               width,
                               height);

}

void Window::render() {
    SDL_RenderClear(sdlRenderer);
    void *pixels;
    int pitch;
    SDL_LockTexture(buffer, NULL, &pixels, &pitch);

    memcpy(pixels, data.data(), pitch * height);
    data = std::vector<uint8_t>(width * height * 4, 0);

    SDL_UnlockTexture(buffer);
    SDL_RenderCopy(sdlRenderer, buffer, NULL, NULL);
    SDL_RenderPresent(sdlRenderer);
}

void Window::draw(int x, int y, int dx, int dy, SDL_Color col) {
    if (dx < 0 || dy < 0 || x < 0 || y < 0 || x + dx >= width || y + dy >= height) {
        std::cerr << "invalid position to draw: " << x << " " << y << "   " << dx << " " << dy << std::endl;
        return;
    }

    std::vector<uint8_t> row;
    for (int j = 0; j < dx; j++) {
        row.emplace_back(col.a);
        row.emplace_back(col.b);
        row.emplace_back(col.g);
        row.emplace_back(col.r);
    }
    for (int i = y; i < dy + y; i++) {
        memcpy(data.data() + (i * width + x) * 4, row.data(), dx * 4);
    }
}

Window::~Window() {
    /* Pauses all SDL subsystems for a variable amount of milliseconds */
    SDL_Delay(DELAY);

    /* Frees memory */
    SDL_DestroyWindow(window);

    /* Shuts down all SDL subsystems */
    SDL_Quit();

}

void Window::drawBorder(int x, int y, int dx, int dy, int w, SDL_Color col) {
    if (dx < 0 || dy < 0 || x < 0 || y < 0 || x + dx >= width || y + dy >= height) {
        std::cerr << "invalid position to draw: " << x << " " << y << "   " << dx << " " << dy << std::endl;
        return;
    }
    if (w > dx || w > dy) {
        std::cerr << "invalid width of boarder: " << w
                  << "  on rectangle of dimensions" << dx << " " << dy << std::endl;
        return;
    }

    std::vector<uint8_t> pixel = {col.a, col.b, col.g, col.r};

    for (int j = x; j < x + dx; j++) {
        for (int i = 0; i < w; i++) {
            memcpy(data.data() + ((y + i) * width + j) * 4, pixel.data(), 4);
            memcpy(data.data() + ((y + dy - i-1) * width + j) * 4, pixel.data(), 4);
        }
    }
    for (int j = y + w; j <= y + dy - w; j++) {
        for (int i = 0; i < w; i++) {
            memcpy(data.data() + (j * width + x + i) * 4, pixel.data(), 4);
            memcpy(data.data() + (j * width + x + dx - i-1) * 4, pixel.data(), 4);
        }
    }


}

