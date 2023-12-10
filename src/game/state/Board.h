//
// Created by Burg on 08.12.2023.
//


#include <vector>
#include <iostream>
#include "../utils/Vec2.h"
#ifndef TETRIS_BOARD_H
#define TETRIS_BOARD_H
#define byte unsigned char

class Board {
    std::vector<std::vector<byte>> board;   // 0 if nothing in spot; otherwise > 0
    byte voidO;

public:
    int width,height;

    Board(int w, int h);

    std::vector<byte> serialize();
    void deserialize(std::vector<byte> &data);

    void debug();

    void reset();       // clears whole board

    void update();      // clears lines


    std::vector<byte>& operator[](int ind) {
        return board[ind];
    };
    byte& operator[](Vec2 vec) {
        if (vec.x < 0 || vec.x >= width || vec.y < 0 || vec.y >= height) {
            std::cerr << "invalid board position " << vec.x << " " << vec.y << std::endl;
            return voidO;
        }
        return board[vec.x][vec.y];
    };
};


#endif //TETRIS_BOARD_H
