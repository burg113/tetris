//
// Created by Burg on 16.12.2023.
//

#include "Game.h"
#include "application/io/input/SDLInputAdapter.h"
#include "networking/BinarySerialize.h"
#include "application/io/output/sdl/TextureRenderer.h"

Game::Game(Tetris *tetris) : tetris(tetris), inputData((int) GameLogic::Key::SIZE), gameLogic() {
    texture= SDL_CreateTexture(tetris->window->getSdlRenderer(),
                               SDL_PIXELFORMAT_RGBA8888,
                               SDL_TEXTUREACCESS_STREAMING,
                               TextureRenderer::width,
                               TextureRenderer::height);

    tetris->window->registerTexture(texture,NULL,NULL);

    SDLInputAdapter::get()->registerCallback([this](bool set, int key) {
        if (KEY_CONVERSION.count(key)) {
            for (int k: KEY_CONVERSION[key]){
                inputData.update(k, set);
            }
        }
    });

    tetris->socket->addReadCallback([this](SocketWrapper* socket, const std::string& s){
        BinaryStream stream(s);

        int frameC = gameLogic.frameCount;
        stream >> this->gameLogic;

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
    inputHistory.emplace_back(gameLogic.frameCount,inputData);
    gameLogic.update(inputData);
    inputData.update(GameLogic::Key::INSTA_DROP, false);
    render();
}

void Game::render() {
    TextureRenderer::render(texture,gameLogic);
    tetris->window->render();
}
