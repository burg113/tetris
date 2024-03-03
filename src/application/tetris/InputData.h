//
// Created by Burg on 03.03.2024.
//

#ifndef TETRIS_INPUTDATA_H
#define TETRIS_INPUTDATA_H

#include "vector"
#include "networking/BinarySerialize.h"

class InputData {
    std::vector<bool> keys;

    friend std::ostream& operator << (std::ostream &s, binary_write_t<InputData> inputAdapter);
    friend std::istream& operator >> (std::istream &s, binary_read_t<InputData> inputAdapter);
};


#endif //TETRIS_INPUTDATA_H
