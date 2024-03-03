//
// Created by Burg on 03.03.2024.
//

#include "InputData.h"

BinaryStream &operator<<(BinaryStream &s, const InputData& inputData) {
    s << inputData.keys;
    return s;
}

InputData::InputData(int size) {
    keys.resize(size);
}

bool InputData::isDown(int keyCode) const {
    return keys[keyCode];
}

void InputData::update(int keyCode, bool state) {
    keys[keyCode] = state;
}

BinaryStream &operator>>(BinaryStream &s, InputData& inputData){
    s >> inputData.keys;
    return s;
}