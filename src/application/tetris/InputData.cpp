//
// Created by Burg on 03.03.2024.
//

#include "InputData.h"

std::ostream &operator<<(std::ostream &s, binary_write_t<InputData> inputAdapter) {
    s << binw(inputAdapter.t.keys);
    return s;
}

std::istream &operator>>(std::istream &s, binary_read_t<InputData> inputAdapter){
    s >> binr(inputAdapter.t.keys);
    return s;
}