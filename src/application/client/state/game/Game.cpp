//
// Created by Burg on 16.12.2023.
//

#include "Game.h"
#include "application/io/input/SDLInputAdapter.h"
#include "networking/BinarySerialize.h"
#include "application/io/output/sdl/TextureRenderer.h"

Game::Game(Tetris *tetris) : tetris(tetris), inputData((int) GameLogic::Key::SIZE), gameLogic() {
    texture = SDL_CreateTexture(tetris->window->getSdlRenderer(),
                                SDL_PIXELFORMAT_RGBA8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                TextureRenderer::width,
                                TextureRenderer::height);

    int size = tetris->window->height;
    dstRect = {0, 0, size / 2, size};
    tetris->window->registerTexture(texture, NULL, &dstRect);

    int dy = size / otherPlayerRows;
    int dx = dy / 2;
    for (int y = 0; y < otherPlayerRows; y++) {
        for (int x = dstRect.x + dstRect.w; x + dx < tetris->window->width; x += dx) {
            otherPlayerRects.emplace_back(SDL_Rect{x, y * dy, dx, dy});
            otherPlayerTextures.emplace_back(SDL_CreateTexture(tetris->window->getSdlRenderer(),
                                                               SDL_PIXELFORMAT_RGBA8888,
                                                               SDL_TEXTUREACCESS_STREAMING,
                                                               TextureRenderer::width,
                                                               TextureRenderer::height));
        }
    }
    for (int i = 0; i < otherPlayerTextures.size(); i++)
        tetris->window->registerTexture(otherPlayerTextures[i], NULL, &otherPlayerRects[i]);

    SDLInputAdapter::get()->registerCallback([this](bool set, int key) {
        if (KEY_CONVERSION.count(key)) {
            for (int k: KEY_CONVERSION[key]) {
                inputData.update(k, set);
            }
        }
    });

    tetris->socket->addReadCallback([this](SocketWrapper *socket, const std::string &s) {
        BinaryStream stream(s);

        int frameC = gameLogic.frameCount;
        stream >> this->gameLogic;
        stream >> otherPlayers;

        while (!inputHistory.empty() && inputHistory.front().first < gameLogic.frameCount) inputHistory.pop_front();

        auto it = inputHistory.begin();
        while (it != inputHistory.end() && gameLogic.frameCount < frameC) gameLogic.update((it++)->second);

    });
}

void Game::sendFrameData() {
    BinaryStream stream;
    stream << ((unsigned char) 0) << gameLogic.frameCount << inputData;
    tetris->socket->send(stream.getData());
}

void Game::update() {
    tetris->socket->getIoService().poll();
    SDLInputAdapter::get()->update();
    sendFrameData();
    inputHistory.emplace_back(gameLogic.frameCount, inputData);
    gameLogic.update(inputData);
    inputData.update(GameLogic::Key::INSTA_DROP, false);
    render();
}

void Game::render() {
    TextureRenderer::render(texture, gameLogic);

    for (int i = 0; i < std::min(otherPlayerTextures.size(), otherPlayers.size()); i++) {
        TextureRenderer::render(otherPlayerTextures[i], otherPlayers[i]);
    }

    tetris->window->render();
}
