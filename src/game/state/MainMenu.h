//
// Created by Burg on 09.12.2023.
//

#ifndef TETRIS_MAINMENU_H
#define TETRIS_MAINMENU_H

#include "State.h"
#include "../Tetris.h"

class MainMenu : public State {
    Tetris* tetris;
public:
    explicit MainMenu(Tetris* tetris);

    void update() override;
    void reset() override;
};


#endif //TETRIS_MAINMENU_H
