package engine

import sdl "vendor:sdl2"

RF32 :: struct {
    x,y,w,h : f32,
}

toRF32 :: proc(rct : sdl.Rect) -> RF32 {
    return RF32{f32(rct.x), f32(rct.y), f32(rct.w), f32(rct.h)}
}

In :: proc(a : sdl.Rect, b : Vec2) -> bool {
    return (
        a.x <= i32(b.x) && 
        a.x + a.w >= i32(b.x) &&
        a.y <= i32(b.y) &&
        a.y + a.h >= i32(b.y)
    )
}

IsIntersecting :: proc(a,b : sdl.Rect) -> bool {
    tmp := RF32{f32(b.x), f32(b.y), f32(b.w), f32(b.h)}
    return (
        In(a, Vec2{tmp.x, tmp.y}) &&
        In(a, Vec2{tmp.x+tmp.w, tmp.y}) &&
        In(a, Vec2{tmp.x, tmp.y+tmp.h}) &&
        In(a, Vec2{tmp.x+tmp.w,tmp.y+tmp.h})
    )
}

Intersection :: proc(a,b : sdl.Rect) -> (sdl.Rect, bool) #optional_ok{
    res := sdl.Rect{
        max(a.x, b.x),
        max(a.y, b.y),
        min(a.x+a.w, b.x+b.w),
        min(a.y+a.h, b.y+b.h),
    }
    res.w -= res.x
    res.h -= res.y
    return res, (res.w >= 0 && res.h >= 0)
}

AddVR :: proc(pos: Vec2, hitbox: sdl.Rect) -> sdl.Rect {
    return {i32(pos.x) + hitbox.x, i32(pos.y) + hitbox.y, hitbox.w, hitbox.h}
}

RectCenter :: proc(el: sdl.Rect) -> Vec2 {
    return {f32(el.x + el.w/2), f32(el.y + el.h/2)}
}