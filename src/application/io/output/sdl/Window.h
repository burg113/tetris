//
// Created by Burg on 08.12.2023.
//

#include <SDL.h>
#ifndef TETRIS_WINDOW_H
#define TETRIS_WINDOW_H
#include <vector>
#include <map>

class Window {
    SDL_Window *window;
    SDL_Renderer *sdlRenderer;
    SDL_Texture *buffer;
public:
    SDL_Renderer *getSdlRenderer(){return sdlRenderer;}
private:
    std::vector<unsigned char> data;

    int IDCount = 0;

    struct RenderObject{
        SDL_Texture* texture;
        const SDL_Rect *dstRect, *srcRect;

        RenderObject(SDL_Texture* texture,const SDL_Rect *srcRect,const SDL_Rect *dstRect) :
                texture(texture), srcRect(srcRect), dstRect(dstRect){};
    };

    std::map<int,RenderObject> renderObjects;

public:
    int width, height;

    bool valid = false;
    Window(int width,int height, const std::string &title);

    void draw(SDL_Texture *texture);
    void draw(int x, int y, int dx, int dy, SDL_Color);
    void drawBorder(int x, int y, int dx, int dy, int width, SDL_Color);

    // returns ID
    int registerTexture(SDL_Texture* texture,const SDL_Rect *srcRect, const SDL_Rect *dstRect);
    void removeTexture(int ind);


    void render();

    ~Window();



};


#endif //TETRIS_WINDOW_H
