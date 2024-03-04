//
// Created by lucas on 09.12.2023.
//

#pragma once

#include <cstdint>
#include <type_traits>
#include <iostream>
#include <vector>
#include <sstream>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

uint64_t htonll(uint64_t x);
uint64_t ntohll(uint64_t x);

class BinaryStream{
    std::stringstream stream;
public:
    explicit BinaryStream(const std::string &data = "");
    // reset the stream and initialize it with data
    void reset(const std::string &data = "");
    std::stringstream& getStringStream();
    std::string getData();
};

// ======================================== Primitives ==========================================

BinaryStream& operator << (BinaryStream &s, const uint8_t& b);
BinaryStream& operator << (BinaryStream &s, const uint16_t& b);
BinaryStream& operator << (BinaryStream &s, const uint32_t& b);
BinaryStream& operator << (BinaryStream &s, const uint64_t& b);
BinaryStream& operator << (BinaryStream &s, const int8_t& b);
BinaryStream& operator << (BinaryStream &s, const int16_t& b);
BinaryStream& operator << (BinaryStream &s, const int32_t& b);
BinaryStream& operator << (BinaryStream &s, const int64_t& b);
BinaryStream& operator << (BinaryStream &s, const bool& b);

BinaryStream& operator >> (BinaryStream &s, uint8_t& b);
BinaryStream& operator >> (BinaryStream &s, uint16_t& b);
BinaryStream& operator >> (BinaryStream &s, uint32_t& b);
BinaryStream& operator >> (BinaryStream &s, uint64_t& b);
BinaryStream& operator >> (BinaryStream &s, int8_t& b);
BinaryStream& operator >> (BinaryStream &s, int16_t& b);
BinaryStream& operator >> (BinaryStream &s, int32_t& b);
BinaryStream& operator >> (BinaryStream &s, int64_t& b);
BinaryStream& operator >> (BinaryStream &s, bool& b);

// ==================================================================================================

template<typename T>
BinaryStream& operator << (BinaryStream &s, const std::vector<T>& b) {
    s << static_cast<uint32_t>(b.size());
    for(T t : b) s << t;
    return s;
}

template<typename T>
BinaryStream& operator >> (BinaryStream &s, std::vector<T>& b) {
    uint32_t size;
    s >> size;
    b.clear();
    b.reserve(size);
    for(uint32_t i = 0; i < size; i++) {
        T t;
        s >> t;
        b.push_back(t);
    }
    return s;
}
