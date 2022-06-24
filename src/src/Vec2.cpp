#pragma once

#include "../include/Vec2.hpp"
#include <cmath>

Vec2::Vec2(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vec2::Vec2(const Vec2& vec)
{
    this->x = vec.x;
    this->y = vec.y;
}

void Vec2::sum(const Vec2& vec)
{
    this->x += vec.x;
    this->y += vec.y;
}

void Vec2::sub(const Vec2& vec)
{
    this->x -= vec.x;
    this->y -= vec.y;
}

void Vec2::mul(const Vec2& vec)
{
    this->x *= vec.x;
    this->y *= vec.y;
}

void Vec2::mulc(const float scalar)
{
    this->x *= scalar;
    this->y *= scalar;
}

void Vec2::divc(const float scalar)
{
    this->x /= scalar;
    this->y /= scalar;
}

float Vec2::mag()
{
    return sqrt(this->x*this->x + this->y*this->y);
}

Vec2 Vec2::norm()
{
    Vec2 vec(this->x, this->y);
    vec.divc(vec.mag());
    return vec;
}

float Vec2::dist(const Vec2& vec)
{
    const float xl = this->x - vec.x;
    const float yl = this->y - vec.y;
    return sqrt(xl*xl + yl*yl);
}

float Vec2::theta()
{
    return atan2(this->y, this->x);
}

float Vec2::theta(const Vec2& vec)
{
    return Vec2::sub(*this, vec).theta();
}

Vec2 Vec2::sum(const Vec2& vecl, const Vec2& vecr)
{
    Vec2 rvec(vecl);
    rvec.sum(vecr);
    return rvec;
}

Vec2 Vec2::sub(const Vec2& vecl, const Vec2& vecr)
{
    Vec2 rvec(vecl);
    rvec.sub(vecr);
    return rvec;
}

Vec2 Vec2::mul(const Vec2& vecl, const Vec2& vecr)
{
    Vec2 rvec(vecl);
    rvec.mul(vecr);
    return rvec;
}

Vec2 Vec2::mulc(const Vec2& vec, const float scalar)
{
    Vec2 rvec(vec);
    rvec.mulc(scalar);
    return rvec;
}

Vec2 Vec2::divc(const Vec2& vec, const float scalar)
{
    Vec2 rvec(vec);
    rvec.divc(scalar);
    return rvec;
}

float Vec2::mag(const Vec2& vec)
{
    return Vec2(vec).mag();
}

Vec2 Vec2::norm(const Vec2& vec)
{
    return Vec2(vec).norm();
}

float Vec2::dist(const Vec2& vecl, const Vec2& vecr)
{
    return Vec2(vecl).dist(vecr);
}

float Vec2::theta1(const Vec2& vec)
{
    return Vec2(vec).theta();
}

float Vec2::theta(const Vec2& vecl, const Vec2& vecr)
{
    return Vec2::sub(vecl, vecr).theta();
}

Vec2& Vec2::Rotate(float angle)
{
    const float cs = cos(angle), sn = sin(angle);
    
    const float xl = this->x*cs - this->y*sn;
    const float yl = this->y*cs + this->x*sn;

    this->x = xl;
    this->y = yl;

    return *this;
}

Vec2& Rotate(Vec2 vec,float angle)
{ return Vec2(vec).Rotate(angle); }

Vec2 Vec2::operator+(const Vec2& vec) { return Vec2::sum(*this,vec); }
Vec2 Vec2::operator-(const Vec2& vec) { return Vec2::sub(*this,vec); }
Vec2 Vec2::operator*(const Vec2& vec) { return Vec2::mul(*this,vec); }
Vec2 Vec2::operator*(const float scalar) { return Vec2::mulc(*this,scalar); }
Vec2 Vec2::operator/(const float scalar) { return Vec2::divc(*this, scalar); }

Vec2& Vec2::operator+=(const Vec2& vec) { this->sum(vec); return *this; }
Vec2& Vec2::operator-=(const Vec2& vec) { this->sub(vec); return *this; }
Vec2& Vec2::operator*=(const float scalar) { this->mulc(scalar); return *this; }
Vec2& Vec2::operator/=(const float scalar) { this->divc(scalar); return *this; }
Vec2& Vec2::operator=(const Vec2& vec) {this->x = vec.x; this->y = vec.y; return *this;}