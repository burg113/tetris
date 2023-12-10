//
// Created by lucas on 09.12.2023.
//

#include "BinarySerialize.h"

uint64_t htonll(uint64_t x)
{
#if IS_BIG_ENDIAN // Note: this is macro is defined in CMakeLists.txt
    return x;
#else
    return ((uint64_t)htonl((x) & 0xFFFFFFFFLL) << 32) | htonl((x) >> 32);
#endif
}

uint64_t ntohll(uint64_t x)
{
#if IS_BIG_ENDIAN
    return x;
#else
    return ((uint64_t)ntohl((x) & 0xFFFFFFFFLL) << 32) | ntohl((x) >> 32);
#endif
}

std::ostream& operator << (std::ostream &s, binary_t<const uint8_t&> b){
    return s.write(reinterpret_cast<const char*>(&b.t), sizeof(b.t));
}
std::ostream& operator << (std::ostream &s, binary_t<const uint16_t&> b){
    uint16_t x = htons(b.t);
    return s.write(reinterpret_cast<const char*>(&x), sizeof(x));
}
std::ostream& operator << (std::ostream &s, binary_t<const uint32_t&> b){
    uint32_t x = htonl(b.t);
    return s.write(reinterpret_cast<char*>(&x), sizeof(x));
}
std::ostream& operator << (std::ostream &s, binary_t<const uint64_t&> b){
    uint64_t x = htonll(b.t);
    return s.write(reinterpret_cast<char*>(&x), sizeof(x));
}

std::ostream& operator << (std::ostream &s, binary_t<const int8_t&> b){
    return s.write(reinterpret_cast<const char*>(&b.t), sizeof(b.t));
}
std::ostream& operator << (std::ostream &s, binary_t<const int16_t&> b){
    uint16_t x = htons(static_cast<uint16_t>(b.t));
    return s.write(reinterpret_cast<const char*>(&x), sizeof(x));
}
std::ostream& operator << (std::ostream &s, binary_t<const int32_t&> b){
    uint32_t x = htonl(static_cast<uint32_t>(b.t));
    return s.write(reinterpret_cast<char*>(&x), sizeof(x));
}
std::ostream& operator << (std::ostream &s, binary_t<const int64_t&> b){
    uint64_t x = htonll(static_cast<uint64_t>(b.t));
    return s.write(reinterpret_cast<char*>(&x), sizeof(x));
}

std::istream& operator >> (std::istream &s, binary_t<uint8_t&> b) {
    return s.read(reinterpret_cast<char*>(&b.t), sizeof(b.t));
}
std::istream& operator >> (std::istream &s, binary_t<uint16_t&> b) {
    auto &ret = s.read(reinterpret_cast<char*>(&b.t), sizeof(b.t));
    b.t = ntohs(b.t);
    return ret;
}
std::istream& operator >> (std::istream &s, binary_t<uint32_t&> b) {
    auto &ret = s.read(reinterpret_cast<char*>(&b.t), sizeof(b.t));
    b.t = ntohl(b.t);
    return ret;
}
std::istream& operator >> (std::istream &s, binary_t<uint64_t&> b) {
    auto &ret = s.read(reinterpret_cast<char*>(&b.t), sizeof(b.t));
    b.t = ntohll(b.t);
    return ret;
}

std::istream& operator >> (std::istream &s, binary_t<int8_t&> b) {
    return s.read(reinterpret_cast<char*>(&b.t), sizeof(b.t));
}
std::istream& operator >> (std::istream &s, binary_t<int16_t&> b) {
    uint16_t x;
    auto &ret = s.read(reinterpret_cast<char*>(&x), sizeof(x));
    b.t = static_cast<int16_t>(ntohs(x));
    return ret;
}
std::istream& operator >> (std::istream &s, binary_t<int32_t&> b) {
    uint32_t x;
    auto &ret = s.read(reinterpret_cast<char*>(&x), sizeof(x));
    b.t = static_cast<int32_t>(ntohl(x));
    return ret;
}
std::istream& operator >> (std::istream &s, binary_t<int64_t&> b) {
    uint64_t x;
    auto &ret = s.read(reinterpret_cast<char*>(&x), sizeof(x));
    b.t = static_cast<int64_t>(ntohll(x));
    return ret;
}