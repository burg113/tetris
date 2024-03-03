//
// Created by Burg on 08.12.2023.
//

#ifndef TETRIS_BOARD_H
#define TETRIS_BOARD_H

#include <vector>
#include <iostream>
#include "utils/Vec2.h"
#include "networking/BinarySerialize.h"

constexpr unsigned char BOARD_INDEX_EMTPY = 255;

class Board {
    std::vector<std::vector<uint8_t>> board;   // 0 if nothing in spot; otherwise > 0
    uint8_t voidO;

public:
    int width,height;

    Board(int w, int h);

    std::vector<uint8_t> serialize();
    void deserialize(std::vector<uint8_t> &data);

    void debug();

    void reset();       // clears whole board

    void update();      // clears lines


    std::vector<uint8_t>& operator[](int ind) {
        return board[ind];
    };
    uint8_t& operator[](Vec2 vec) {
        if (vec.x < 0 || vec.x >= width || vec.y < 0 || vec.y >= height) {
            std::cerr << "invalid board position " << vec.x << " " << vec.y << std::endl;
            return voidO;
        }
        return board[vec.x][vec.y];
    };


    friend std::ostream& operator << (std::ostream &s, binary_write_t<Board> b);
    friend std::istream& operator >> (std::istream &s, binary_read_t<Board> b);
};


#endif //TETRIS_BOARD_H
