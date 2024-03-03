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

BinaryStream& operator << (BinaryStream &s, const uint8_t& b){
    s.getStringStream().write(reinterpret_cast<const char*>(&b), sizeof(b));
    return s;
}
BinaryStream& operator << (BinaryStream &s, const uint16_t& b){
    uint16_t x = htons(b);
    s.getStringStream().write(reinterpret_cast<const char*>(&x), sizeof(x));
    return s;
}
BinaryStream& operator << (BinaryStream &s, const uint32_t& b){
    uint32_t x = htonl(b);
    s.getStringStream().write(reinterpret_cast<char*>(&x), sizeof(x));
    return s;
}
BinaryStream& operator << (BinaryStream &s, const uint64_t& b){
    uint64_t x = htonll(b);
    s.getStringStream().write(reinterpret_cast<char*>(&x), sizeof(x));
    return s;
}

BinaryStream& operator << (BinaryStream &s, const int8_t& b){
    s.getStringStream().write(reinterpret_cast<const char*>(&b), sizeof(b));
    return s;
}
BinaryStream& operator << (BinaryStream &s, const int16_t& b){
    uint16_t x = htons(static_cast<uint16_t>(b));
    s.getStringStream().write(reinterpret_cast<const char*>(&x), sizeof(x));
    return s;
}
BinaryStream& operator << (BinaryStream &s, const int32_t& b){
    uint32_t x = htonl(static_cast<uint32_t>(b));
    s.getStringStream().write(reinterpret_cast<char*>(&x), sizeof(x));
    return s;
}
BinaryStream& operator << (BinaryStream &s, const int64_t& b){
    uint64_t x = htonll(static_cast<uint64_t>(b));
    s.getStringStream().write(reinterpret_cast<char*>(&x), sizeof(x));
    return s;
}
BinaryStream& operator << (BinaryStream &s, const bool& b){
    s << (uint8_t)b;
    return s;
}

BinaryStream& operator >> (BinaryStream &s, uint8_t& b) {
    s.getStringStream().read(reinterpret_cast<char*>(&b), sizeof(b));
    return s;
}
BinaryStream& operator >> (BinaryStream &s, uint16_t& b) {
    s.getStringStream().read(reinterpret_cast<char*>(&b), sizeof(b));
    b = ntohs(b);
    return s;
}
BinaryStream& operator >> (BinaryStream &s, uint32_t& b) {
    s.getStringStream().read(reinterpret_cast<char*>(&b), sizeof(b));
    b = ntohl(b);
    return s;
}
BinaryStream& operator >> (BinaryStream &s, uint64_t& b) {
    s.getStringStream().read(reinterpret_cast<char*>(&b), sizeof(b));
    b = ntohll(b);
    return s;
}

BinaryStream& operator >> (BinaryStream &s, int8_t& b) {
    s.getStringStream().read(reinterpret_cast<char*>(&b), sizeof(b));
    return s;
}
BinaryStream& operator >> (BinaryStream &s, int16_t& b) {
    uint16_t x;
    s.getStringStream().read(reinterpret_cast<char*>(&x), sizeof(x));
    b = static_cast<int16_t>(ntohs(x));
    return s;
}
BinaryStream& operator >> (BinaryStream &s, int32_t& b) {
    uint32_t x;
    s.getStringStream().read(reinterpret_cast<char*>(&x), sizeof(x));
    b = static_cast<int32_t>(ntohl(x));
    return s;
}
BinaryStream& operator >> (BinaryStream &s, int64_t& b) {
    uint64_t x;
    s.getStringStream().read(reinterpret_cast<char*>(&x), sizeof(x));
    b = static_cast<int64_t>(ntohll(x));
    return s;
}
BinaryStream& operator >> (BinaryStream &s, bool& b){
    uint8_t x;
    s >> x;
    b = static_cast<bool>(x);
    return s;
}

std::string BinaryStream::getData() {
    return stream.str();
}

std::stringstream &BinaryStream::getStringStream() {
    return stream;
}

BinaryStream::BinaryStream(const std::string &data) : stream(data) {

}

void BinaryStream::reset(const std::string &data) {
    stream.str(data);
    stream.clear();
}
