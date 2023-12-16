//
// Created by Burg on 09.12.2023.
//

#ifndef TETRIS_APPLICATIONSTATE_H
#define TETRIS_APPLICATIONSTATE_H


class ApplicationState {
public:
    virtual void update() = 0;

    virtual ~ApplicationState() = default;

};


#endif //TETRIS_APPLICATIONSTATE_H
