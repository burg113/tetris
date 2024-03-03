//
// Created by Burg on 03.03.2024.
//

#ifndef TETRIS_INPUTDATA_H
#define TETRIS_INPUTDATA_H

#include "vector"
#include "networking/BinarySerialize.h"

class InputData {

    std::vector<bool> keys;

public:

    explicit InputData() = default;
    explicit InputData(int size);

    bool isDown(int keyCode) const;

    void update(int keyCode,bool state);

    friend BinaryStream& operator << (BinaryStream &s, const InputData& inputAdapter);
    friend BinaryStream& operator >> (BinaryStream &s, InputData& inputAdapter);
};


#endif //TETRIS_INPUTDATA_H
