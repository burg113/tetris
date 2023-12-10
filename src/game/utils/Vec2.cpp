//
// Created by Burg on 09.12.2023.
//

#include "Vec2.h"

Vec2::Vec2(int x, int y) : x(x), y(y) {}
Vec2::Vec2() : x(0), y(0) {}


Vec2 Vec2::operator+(const Vec2 &other) const {
    return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator+=(const Vec2 &other) {
    x += other.x, y += other.y;
    return *this;
}

Vec2 Vec2::operator-(const Vec2 &other) const {
    return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator-=(const Vec2 &other) {
    x -= other.x, y -= other.y;
    return *this;
}

Vec2 Vec2::operator*(int num) const {
    return Vec2(x * num, y * num);
}

Vec2 Vec2::operator*=(int num) {
    x*=num, y*= num;
    return *this;
}

Vec2 Vec2::operator/(int num) const {
    return Vec2(x / num, y / num);
}
Vec2 Vec2::operator/=(int num) {
    x/=num, y/= num;
    return *this;
}
Vec2 Vec2::rotL() {
    return Vec2(-y, x);
}

Vec2 Vec2::rotR() {
    return Vec2(y, -x);
}

Vec2 Vec2::rot(int i) {
    if ((i%4+4)%4 == 0) return *this;
    if ((i%4+4)%4 == 1) return rotL();
    if ((i%4+4)%4 == 2) return *this * (-1);
    return rotR();
}



