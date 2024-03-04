//
// Created by Burg on 08.12.2023.
//
#pragma once


#include <SDL.h>
#include <vector>
#include <map>

class Window {
    SDL_Window *window;
    SDL_Renderer *sdlRenderer;

public:
    SDL_Renderer *getSdlRenderer() { return sdlRenderer; }

private:

    int IDCount = 0;

    struct RenderObject {
        SDL_Texture *texture;
        const SDL_Rect *dstRect, *srcRect;

        RenderObject(SDL_Texture *texture, const SDL_Rect *srcRect, const SDL_Rect *dstRect) :
                texture(texture), srcRect(srcRect), dstRect(dstRect) {};
    };

    std::map<int, RenderObject> renderObjects;

public:
    int width, height;

    bool valid = false;

    Window(int width, int height, const std::string &title);

    // returns ID
    int registerTexture(SDL_Texture *texture, const SDL_Rect *srcRect, const SDL_Rect *dstRect);

    [[maybe_unused]] void removeTexture(int ind);


    void render();

    ~Window();


};

