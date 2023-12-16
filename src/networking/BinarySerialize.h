//
// Created by lucas on 09.12.2023.
//

#ifndef TETRIS_BINARYSERIALIZE_H
#define TETRIS_BINARYSERIALIZE_H

#include <cstdint>
#include <type_traits>
#include <iostream>
#include <vector>

#ifdef _WIN32
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif

uint64_t htonll(uint64_t x);
uint64_t ntohll(uint64_t x);



template<typename T> struct binary_write_t { const T &t; };
template<typename T> struct binary_read_t { T &t; };
template<typename T> binary_write_t<T> binw(const T& t) { return binary_write_t<T>{t}; }
template<typename T> binary_read_t<T> binr(T &t) { return binary_read_t<T>{t}; }

// ======================================== Primitives ==========================================

std::ostream& operator << (std::ostream &s, binary_write_t<uint8_t> b);
std::ostream& operator << (std::ostream &s, binary_write_t<uint16_t> b);
std::ostream& operator << (std::ostream &s, binary_write_t<uint32_t> b);
std::ostream& operator << (std::ostream &s, binary_write_t<uint64_t> b);
std::ostream& operator << (std::ostream &s, binary_write_t<int8_t> b);
std::ostream& operator << (std::ostream &s, binary_write_t<int16_t> b);
std::ostream& operator << (std::ostream &s, binary_write_t<int32_t> b);
std::ostream& operator << (std::ostream &s, binary_write_t<int64_t> b);
std::ostream& operator << (std::ostream &s, binary_write_t<bool> b);

std::istream& operator >> (std::istream &s, binary_read_t<uint8_t> b);
std::istream& operator >> (std::istream &s, binary_read_t<uint16_t> b);
std::istream& operator >> (std::istream &s, binary_read_t<uint32_t> b);
std::istream& operator >> (std::istream &s, binary_read_t<uint64_t> b);
std::istream& operator >> (std::istream &s, binary_read_t<int8_t> b);
std::istream& operator >> (std::istream &s, binary_read_t<int16_t> b);
std::istream& operator >> (std::istream &s, binary_read_t<int32_t> b);
std::istream& operator >> (std::istream &s, binary_read_t<int64_t> b);
std::istream& operator >> (std::istream &s, binary_read_t<bool> b);

// ==================================================================================================

template<typename T>
std::ostream& operator << (std::ostream &s, binary_write_t<std::vector<T>> b) {
    s << binw(static_cast<uint32_t>(b.t.size()));
    for(T t : b.t) s << binw(t);
    return s;
}

template<typename T>
std::istream& operator >> (std::istream &s, binary_read_t<std::vector<T>> b) {
    uint32_t size;
    s >> binr(size);
    b.t.clear();
    b.t.reserve(size);
    for(uint32_t i = 0; i < size; i++) {
        T t;
        s >> binr(t);
        b.t.push_back(t);
    }
    return s;
}

#endif //TETRIS_BINARYSERIALIZE_H
