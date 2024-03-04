//
// Created by Burg on 04.03.2024.
//

#include <cassert>
#include "spdlog/spdlog.h"
#include "TextureRenderer.h"

const std::vector<SDL_Color> colors = {{20,  20,  200, 255},
                                       {240, 180, 20,  255},
                                       {15,  230, 15,  255},
                                       {140, 20,  180, 255},
                                       {200, 20,  20,  255},
                                       {20,  200, 240, 255},
                                       {215, 220, 0,   255}};

void draw(int x, int y, int dx, int dy, SDL_Color col, std::vector<uint8_t> &data) {
    if (dx < 0 || dy < 0 || x < 0 || y < 0 || x + dx >= TextureRenderer::width || y + dy >= TextureRenderer::height) {
        SPDLOG_WARN("invalid positions to draw: {] {} {} {}", x, y, dx, dy);
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
        memcpy(data.data() + (i * TextureRenderer::width + x) * 4, row.data(), dx * 4);
    }
}

void drawBorder(int x, int y, int dx, int dy, int w, SDL_Color col, std::vector<uint8_t> &data) {
    if (dx < 0 || dy < 0 || x < 0 || y < 0 || x + dx >= TextureRenderer::width || y + dy >= TextureRenderer::height) {
        SPDLOG_WARN("invalid positions to draw: {] {} {} {}", x, y, dx, dy);
        return;
    }
    if (w > dx || w > dy) {
        SPDLOG_WARN("invalid width of boarder: {} on rectangle of dimensions {} {}", w, dx, dy);
        return;
    }

    std::vector<uint8_t> pixel = {col.a, col.b, col.g, col.r};

    for (int j = x; j < x + dx; j++) {
        for (int i = 0; i < w; i++) {
            memcpy(data.data() + ((y + i) * TextureRenderer::width + j) * 4, pixel.data(), 4);
            memcpy(data.data() + ((y + dy - i - 1) * TextureRenderer::width + j) * 4, pixel.data(), 4);
        }
    }
    for (int j = y + w; j <= y + dy - w; j++) {
        for (int i = 0; i < w; i++) {
            memcpy(data.data() + (j * TextureRenderer::width + x + i) * 4, pixel.data(), 4);
            memcpy(data.data() + (j * TextureRenderer::width + x + dx - i - 1) * 4, pixel.data(), 4);
        }
    }


}

std::vector<uint8_t> TextureRenderer::renderToVec(GameLogic &gameLogic) {
    std::vector<uint8_t> data = std::vector<uint8_t>(width * height * 4, 0);
    Board *board = &gameLogic.board;
    float fill = .9;
    int size = std::min(width / board->width, height / board->height) * fill;

    Vec2 upperCorner(width / 2 - size * board->width / 2,
                     height / 2 - size * board->height / 2);
    for (int y = 0; y < board->height; y++) {
        for (int x = 0; x < board->width; x++) {
            SDL_Color col;

            if ((*board)[x][y] == BOARD_INDEX_EMTPY) {
                col = {10, 10, 10, 255};
                draw(upperCorner.x + size * x, upperCorner.y + size * y, size, size, col, data);
            } else {
                col = colors[(*board)[x][y]];
                col.r *= .8;
                col.g *= .8;
                col.b *= .8;
                draw(upperCorner.x + size * x, upperCorner.y + size * y, size, size, col, data);

                col.r *= .6;
                col.g *= .6;
                col.b *= .6;
                drawBorder(upperCorner.x + size * x, upperCorner.y + size * y, size, size,
                           boarderWidth, col, data);
            }
        }
    }

    Vec2 offset(0, size - (size * gameLogic.framesToFall) / gameLogic.framesPerFall);
    if (!gameLogic.checkOffset(Vec2(0, 1)) || !smoothAnimation) offset = Vec2(0, 0);

    for (Vec2 v: gameLogic.pieces[gameLogic.pieceInd].tiles) {
        Vec2 p = upperCorner + offset + (v.rot(gameLogic.rotation) + gameLogic.position) * size;
        draw(p.x, p.y, size, size, colors[gameLogic.pieces[gameLogic.pieceInd].colorId], data);
    }

    return data;
}


void TextureRenderer::render(SDL_Texture *texture, GameLogic &gameLogic) {
    int x,y;
    SDL_QueryTexture(texture, NULL, NULL, &x, &y);

    assert(x == TextureRenderer::width && y == TextureRenderer::height);

    void *pixels;
    int pitch;
    SDL_LockTexture(texture, NULL, &pixels, &pitch);

    std::vector<uint8_t> data = renderToVec(gameLogic);

    memcpy(pixels, data.data(), pitch * height);
    SDL_UnlockTexture(texture);

}
