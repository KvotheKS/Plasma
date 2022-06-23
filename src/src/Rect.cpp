#pragma once

#include "../include/Rect.hpp"

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

Vec2 Rect::center(const Rect& rect) { return Rect(rect).center();}
    
float Rect::centerDist(const Rect& rectl, const Rect& rectr)
{ return Rect(rectl).center().dist(Rect(rectr).center()); }

bool Rect::in(const Rect& rect, const Vec2& vec)
{ return Rect(rect).in(vec); }