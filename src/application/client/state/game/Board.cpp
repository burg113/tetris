//
// Created by Burg on 08.12.2023.
//


#include <iostream>
#include "spdlog/spdlog.h"
#include "Board.h"

using namespace std;

Board::Board(int w, int h) : width(w), height(h) {
    reset();
}

void Board::debug() {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            cerr << 'a' + board[x][y] << " ";
        }
        cerr << endl;
    }

}

void Board::reset() {
    board = vector < vector < uint8_t >> (width, vector<uint8_t>(height, BOARD_INDEX_EMTPY));
}

void Board::update() {
    for (int y = 0; y < height; y++) {
        int clear = true;
        for (int x = 0; x < width; x++) {
            if (board[x][y] == BOARD_INDEX_EMTPY) clear = false;
        }
        if (clear) {
            for (int x = 0; x < width; x++) {
                for (int tmpY = y; tmpY > 0; tmpY--) {
                    board[x][tmpY] = board[x][tmpY -1];
                }

                board[x][0] = BOARD_INDEX_EMTPY;
            }
        }
    }

}

std::vector<uint8_t>& Board::operator[](int ind) {
    return board[ind];
};

uint8_t& Board::operator[](Vec2 vec) {
    if (vec.x < 0 || vec.x >= width || vec.y < 0 || vec.y >= height) {
        SPDLOG_WARN("invalid board position {} {}", vec.x, vec.y);
        return voidO;
    }
    return board[vec.x][vec.y];
};


BinaryStream &operator<<(BinaryStream &s, const Board& b) {
    s << b.board;
    return s;
}

BinaryStream &operator>>(BinaryStream &s, Board& b) {
    s >> b.board;
    return s;
}



