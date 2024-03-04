//
// Created by Burg on 08.12.2023.
//

#pragma once


#include <vector>
#include <iostream>
#include "utils/Vec2.h"
#include "networking/BinarySerialize.h"

constexpr unsigned char BOARD_INDEX_EMTPY = 255;

class Board {
    std::vector<std::vector<uint8_t>> board;   // 0 if nothing in spot; otherwise > 0
    uint8_t voidO = 0;

public:
    int width,height;

    Board(int w, int h);

    void debug();

    void reset();       // clears whole board

    void update();      // clears lines


    std::vector<uint8_t>& operator[](int ind);
    uint8_t& operator[](Vec2 vec);


    friend BinaryStream& operator << (BinaryStream &s, const Board& b);
    friend BinaryStream& operator >> (BinaryStream &s, Board& b);
};

