#pragma once
#include "decl.hpp"

class Vec2
{
public:
    float x, y;

public:
    Vec2(float x = 0.0f, float y = 0.0f);
    Vec2(const Vec2& vec);

public:
    void sum(const Vec2& vec);
    void sub(const Vec2& vec);
    void mulc(const float scalar);
    void divc(const float scalar);
    float mag();
    Vec2 norm();
    float dist(const Vec2& vec);
    float theta();
    float theta(const Vec2& vec);
    Vec2& Rotate(float angle);

public:
    Vec2 sum(const Vec2& vecl, const Vec2& vecr);
    Vec2 sub(const Vec2& vecl, const Vec2& vecr);
    Vec2 mulc(const Vec2& vec, const float scalar);
    Vec2 divc(const Vec2& vec, const float scalar);
    float mag(const Vec2& vec);
    Vec2 norm(const Vec2& vec);
    float dist(const Vec2& vecl, const Vec2& vecr);
    float theta1(const Vec2& vec);
    float theta(const Vec2& vecl, const Vec2& vecr);
    Vec2& Rotate(Vec2 vec,float angle);

public:
    Vec2 operator+(const Vec2& vec);
    Vec2 operator-(const Vec2& vec);
    Vec2 operator*(const float scalar);
    Vec2 operator/(const float scalar);

    Vec2& operator+=(const Vec2& vec);
    Vec2& operator-=(const Vec2& vec);
    Vec2& operator*=(const float scalar);
    Vec2& operator/=(const float scalar);
};