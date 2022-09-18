package engine

import sdl "vendor:sdl2"
import math "core:math"

Vec2 :: [2]f32

floor :: proc(a : f32) -> i64 {
    return i64(a)
}

ceil :: proc(a : f32) -> i64 {
    return i64(a) + (1 if f32(i64(a)) < a else 0)
}

Mag :: proc(vec : Vec2) -> f32 {
    return math.sqrt(vec.x*vec.x + vec.y*vec.y)
}

Rotate :: proc(vec: Vec2, angle : f32) -> Vec2 {
    cs := math.cos(angle)
    sn := math.sin(angle)
    xl := vec.x*cs - vec.y*sn
    yl := vec.y*cs + vec.x*sn
    return Vec2{xl,yl}
}

Theta :: proc(vec: Vec2) -> f32 {
    return math.atan2(vec.y, vec.x)
}

Norm :: proc(vec: Vec2) -> Vec2 {
    tmp := Mag(vec)
    return vec/tmp if tmp != 0 else 0
}

Dist :: proc(veca, vecb : Vec2) -> f32 {
    return Mag(veca-vecb)
}

AssertAngle :: proc(angle: ^f32) {
    angle^ -= math.TAU if angle^ > math.TAU else 0
    angle^ += math.TAU if angle^ < 0 else 0
}

Dot :: proc(veca, vecb: Vec2) -> f32 {
    return veca.x*vecb.x + veca.y*vecb.y
}