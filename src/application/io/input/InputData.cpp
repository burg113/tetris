//
// Created by Burg on 03.03.2024.
//

#include "InputData.h"

std::ostream &operator<<(std::ostream &s, binary_write_t<InputData> inputAdapter) {
    s << binw(inputAdapter.t.keys);
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

std::istream &operator>>(std::istream &s, binary_read_t<InputData> inputAdapter){
    s >> binr(inputAdapter.t.keys);
    return s;
}