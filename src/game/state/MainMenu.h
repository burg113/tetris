//
// Created by Burg on 09.12.2023.
//

#ifndef TETRIS_MAINMENU_H
#define TETRIS_MAINMENU_H

#include "ApplicationState.h"
#include "../Tetris.h"

class MainMenu : public ApplicationState {
    Tetris *tetris;

    void reset();

public:
    explicit MainMenu(Tetris *tetris);

    void update() override;
};


#endif //TETRIS_MAINMENU_H
