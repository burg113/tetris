//
// Created by Burg on 04.03.2024.
//

#ifndef TETRIS_TEXTURERENDERER_H
#define TETRIS_TEXTURERENDERER_H

#include <SDL.h>
#include "application/tetris/GameLogic.h"


class TextureRenderer {
private:
    static std::vector<uint8_t> renderToVec(GameLogic &gameLogic);

public:
    static constexpr int width = 500, height = 1000;
    static constexpr int boarderWidth = 3;
    static constexpr bool smoothAnimation = true;

    static void render(SDL_Texture *texture, GameLogic &game);



};


#endif //TETRIS_TEXTURERENDERER_H
