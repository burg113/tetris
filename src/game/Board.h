//
// Created by Burg on 08.12.2023.
//


#include <vector>

#ifndef TETRIS_BOARD_H
#define TETRIS_BOARD_H

#define byte c
class Board {
    int w,h;
    std::vector<std::vector<char>> board;
public:
    Board(int w, int h);

    vector<char> serialize();
    void deserialize(vector<char> &data);

    void debug();
};


#endif //TETRIS_BOARD_H
