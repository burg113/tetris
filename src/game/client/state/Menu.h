//
// Created by Burg on 09.12.2023.
//

#ifndef TETRIS_MENU_H
#define TETRIS_MENU_H

#include "ApplicationState.h"
#include "../Tetris.h"

class Menu : public ApplicationState {
    Tetris *tetris;

    void reset();

public:
    explicit Menu(Tetris *tetris);

    void update() override;
};


#endif //TETRIS_MENU_H
