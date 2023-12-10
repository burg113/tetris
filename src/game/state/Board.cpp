//
// Created by Burg on 08.12.2023.
//

using namespace std;

#include <iostream>
#include "Board.h"


Board::Board(int w, int h) : width(w), height(h) {
    reset();
}

vector<uint8_t> Board::serialize() {
    vector<uint8_t> outBuff;

    outBuff.emplace_back(width);
    outBuff.emplace_back(width >> 8);
    outBuff.emplace_back(height);
    outBuff.emplace_back(height >> 8);

    outBuff.reserve(width * height);
    for (vector<uint8_t> &col: board)
        for (uint8_t b: col)
            outBuff.emplace_back(b);
    return outBuff;
}

void Board::deserialize(vector<uint8_t> &data) {
    if (data.size() < 4) {
        cerr << "To few bytes in data stream! Expected at least 4 but recived" << data.size() << endl;
        return;
    }
    width = data[0] + (data[1] << 8);
    height = data[2] + (data[3] << 8);
    if (data.size() != width * height + 4) {
        cerr << "Incorrect amount of bytes in data stream! Expected " << width * height + 4 << " but received "
             << data.size() << endl;
        return;
    }
    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            board[x][y] = data[y * height + x];
        }
    }
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
    board = vector < vector < uint8_t >> (width, vector<uint8_t>(height, 0));;
}

void Board::update() {
    for (int y = 0; y < height; y++) {
        int clear = true;
        for (int x = 0; x < width; x++) {
            if (board[x][y] == 0) clear = false;
        }
        if (clear) {
            for (int x = 0; x < width; x++) {
                for (int tmpY = y; tmpY > 0; tmpY--) {
                    board[x][tmpY] = board[x][tmpY -1];
                }
                board[x][0] = 0;
            }
        }
    }

}



