//
// Created by Burg on 09.12.2023.
//

#pragma once

#include "application/client/state/ApplicationState.h"
#include "application/client/Tetris.h"

class Menu : public ApplicationState {
    Tetris *tetris;

    void reset();

public:
    explicit Menu(Tetris *tetris);

    void update() override;
};

