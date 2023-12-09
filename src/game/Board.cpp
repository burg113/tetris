//
// Created by Burg on 08.12.2023.
//

using namespace std;

#include <iostream>
#include "Board.h"
#define byte char


Board::Board(int w, int h) : w(w), h(h) {
    board = vector<vector<byte>>(w,vector<byte>(0));




}

vector<char> Board::serialize() {
    vector<byte> outBuff;

    outBuff.emplace_back(w);
    outBuff.emplace_back(w >> 8);
    outBuff.emplace_back(h);
    outBuff.emplace_back(h >> 8);

    outBuff.reserve(w*h);
    for(vector<byte>& col : board)
        for(byte b : col)
            outBuff.emplace_back(b);
    return outBuff;
}

void Board::deserialize(vector<char> &data) {
    if (data.size() < 4) {
        cerr << "To few bytes in data stream! Expected at least 4 but recived" << data.size()<<endl;
        return;
    }
    w = data[0] + (data[1] << 8);
    h = data[2] + (data[3] << 8);
    if (data.size() != w*h + 4) {
        cerr << "Incorrect amount of bytes in data stream! Expected " <<w*h+4 <<" but received " << data.size()<<endl;
        return;
    }
    for(int x = 0;x<w;x++) {
        for(int y = 0;y<h;y++) {
            board[x][y] = data[y*h+x];
        }
    }
}

void Board::debug() {
    for(int y = 0;y<h;y++) {
        for(int x = 0;x<w;x++) {
            cerr << 'a' + board[x][y] << " ";
        }
        cerr << endl;
    }

}
