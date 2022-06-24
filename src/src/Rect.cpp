#pragma once

#include "../include/Rect.hpp"
#include <algorithm>

Rect::Rect(float x, float y, float w, float h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

Rect::Rect(const Rect& rect) 
{ Rect(rect.x,rect.y,rect.w,rect.h); }

Vec2 Rect::center()
{
    return Vec2(this->x + (this->w/2), this->y + (this->h/2));
}

float Rect::centerDist(const Rect& rect)
{
    return abs(this->center().dist(Rect(rect).center()));
}

bool Rect::in(const Vec2& vec)
{
    return (this->x <= vec.x && this->x + this->w >= vec.x) &&
           (this->y <= vec.y && this->y + this->h >= vec.y);
}

bool Rect::intersect(const Rect& rect)
{
    return in(Vec2(rect.x, rect.y)) ||
           in(Vec2(rect.x + rect.w, rect.y)) ||
           in(Vec2(rect.x, rect.y + rect.h)) ||
           in(Vec2(rect.x + rect.w, rect.y + rect.h));
}

Rect Rect::intersection(const Rect& rect)
{
    
    const float xl = std::max(rect.x, this->x);    
    const float yl = std::max(rect.y, this->y);
    const float wl = std::min(rect.x+rect.w, this->x+this->w) - xl;
    const float hl = std::min(rect.y+rect.h, this->y+this->h) - yl;
    
    return {xl, yl, wl, hl};
}

Vec2 Rect::center(const Rect& rect) { return Rect(rect).center();}
    
void Rect::setCenter(const Vec2& vec)
{
    this->x = vec.x - this->w;
    this->y = vec.y - this->h;
}
    
float Rect::centerDist(const Rect& rectl, const Rect& rectr)
{ return Rect(rectl).center().dist(Rect(rectr).center()); }

bool Rect::in(const Rect& rect, const Vec2& vec)
{ return Rect(rect).in(vec); }

Rect Rect::copy() { return {this->x, this->y, this->w, this->h}; }

Rect& Rect::operator=(const Rect& rect)
{
    this->x = rect.x;
    this->y = rect.y;
    this->w = rect.w;
    this->h = rect.h;
    return *this;
}