#pragma once

#include "./decl.hpp"
#include "./Vec2.hpp"
#include <cmath>

class Rect 
{
public:
    float x, y, w, h;

public:
    Rect(float x = 0.0f, float y = 0.0f, float w = 0.0f, float h = 0.0f);
    Rect(const Rect& rect);

public:
    Vec2 center();
    float centerDist(const Rect& rect);
    bool in(const Vec2& vec);

public:
    static Vec2 center(const Rect& rect);
    
    static float centerDist(const Rect& rectl, const Rect& rectr);

    static bool in(const Rect& rect, const Vec2& vec);
};