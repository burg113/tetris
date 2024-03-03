//
// Created by Burg on 08.12.2023.
//

#include <SDL.h>
#ifndef TETRIS_WINDOW_H
#define TETRIS_WINDOW_H
#include <vector>

class Window {
    SDL_Window *window;
    SDL_Renderer *sdlRenderer;
    SDL_Texture *buffer;

    std::vector<unsigned char> data;

public:
    int width, height;

    bool valid = false;
    Window(int width,int height, const std::string &title);

    void draw(int x, int y, int dx, int dy, SDL_Color);
    void drawBorder(int x, int y, int dx, int dy, int width, SDL_Color);

    void render();

    ~Window();


};


#endif //TETRIS_WINDOW_H
