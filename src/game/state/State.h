//
// Created by Burg on 09.12.2023.
//

#ifndef TETRIS_STATE_H
#define TETRIS_STATE_H


class State {
public:
    virtual void reset() = 0;

    virtual void update() = 0;


    virtual ~State() = default;

};


#endif //TETRIS_STATE_H
