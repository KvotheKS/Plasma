package engine

import sdl "vendor:sdl2"
import "core:fmt"
import math "core:math"

Camera :: struct {
    pos, size : Vec2,
    disloc : Vec2,
    speed : Vec2,
    follow : ^Vec2,
    offset : Vec2,
    depth : f32,
}

__camera : Camera;

InitializeCamera :: proc() {
    __camera.size = __ctx.size
    __camera.speed = Vec2{500,500}
}

ObjectInCamera :: proc(obj : sdl.Rect) -> (sdl.Rect, bool) #optional_ok {
    tmp := sdl.Rect{
        i32(__camera.pos.x),
        i32(__camera.pos.y), 
        i32(__camera.size.x), 
        i32(__camera.size.y),
    }
    return Intersection(tmp,obj)
}

MoveCameraKeys :: proc() {
    dt := DeltaTime()
    v2 : Vec2
    
    __camera.follow = nil

    if(IsKeyDown(sdl.Scancode.A)) {
        v2.x -= 1
    }
    if(IsKeyDown(sdl.Scancode.D)) {
        v2.x += 1
    }
    if(IsKeyDown(sdl.Scancode.W)) {
        v2.y -= 1
    }
    if(IsKeyDown(sdl.Scancode.S)) {
        v2.y += 1
    }
    __camera.disloc = dt*v2*__camera.speed;
    __camera.pos += __camera.disloc
}

MoveKeysParallax :: proc(pos : ^Vec2, depth : f32) {
    pos^ += __camera.disloc*(1-math.pow(2,-depth))
}

SetCameraFollow :: proc(follow : ^Vec2, offset : Vec2 = {0,0}, depth : f32 = 0) {
    __camera.follow = follow
    __camera.offset = offset
    __camera.depth = depth
}

MoveCameraFollower :: proc() {
    tmp := __camera.pos
    __camera.pos = __camera.follow^ + __camera.offset - __camera.size/2
    __camera.disloc = __camera.pos - tmp
}

MoveFollowerParallax :: proc(pos: ^Vec2, depth : f32) {
    // fmt.println(1-math.pow(2,-depth + __camera.depth))
    pos^ += __camera.disloc*(1-math.pow(2,-depth + __camera.depth))
}