//
// Created by Burg on 09.12.2023.
//

#ifndef TETRIS_MENU_H
#define TETRIS_MENU_H

#include "State.h"
#include "../Tetris.h"

class Menu : public State{
    Tetris* tetris;
public:
    explicit Menu(Tetris* tetris);

    void update() override;
    void reset() override;

};


#endif //TETRIS_MENU_H
