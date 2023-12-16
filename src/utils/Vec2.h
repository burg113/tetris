//
// Created by Burg on 09.12.2023.
//

#ifndef TETRIS_VEC2_H
#define TETRIS_VEC2_H


#include <complex>

class Vec2 {
public:
    int x, y;

    explicit Vec2(int x, int y);

    Vec2();

    Vec2 operator+(const Vec2 &rhs) const;

    Vec2 operator+=(const Vec2 &rhs);

    Vec2 operator-(const Vec2 &rhs) const;

    Vec2 operator-=(const Vec2 &rhs);

    Vec2 operator*(int num) const;

    Vec2 operator*=(int num);
    Vec2 operator/(int num) const;

    Vec2 operator/=(int num);

    Vec2 rotL();

    Vec2 rotR();

    Vec2 rot(int i);
};


#endif //TETRIS_VEC2_H
