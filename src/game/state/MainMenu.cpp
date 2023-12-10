//
// Created by Burg on 09.12.2023.
//

#include "MainMenu.h"

void MainMenu::update() {
    tetris->gotoState(GAME);
}

void MainMenu::reset() {

}

MainMenu::MainMenu(Tetris *tetris) : tetris(tetris){
    reset();
}
