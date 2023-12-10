//
// Created by Burg on 09.12.2023.
//

#include "Menu.h"

void Menu::update() {
    tetris->gotoState(GAME);
}

void Menu::reset() {

}

Menu::Menu(Tetris *tetris) : tetris(tetris) {
    reset();
}

