package engine

import sdl "vendor:sdl2"
import slc "core:slice"

IsCollidingBase :: proc(rectA, rectB : sdl.Rect, angleA, angleB: f32) -> bool {
    left := toRF32(rectA)
    right := toRF32(rectB)

    A : []Vec2 = {
        {left.x, left.y + left.h},
        {left.x + left.w, left.y + left.h},
        {left.x + left.w, left.y},
        {left.x,left.y}
    }
    B : []Vec2 = {
        {right.x, right.y + right.h},
        {right.x + right.w, right.y + right.h},
        {right.x + right.w, right.y},
        {right.x,right.y}
    }

    for val in &A {
        val = Rotate(val - RectCenter(rectA), angleA) + RectCenter(rectA)
    }

    for val in &B {
        val = Rotate(val - RectCenter(rectB), angleB) + RectCenter(rectB)
    }

    axes : []Vec2 = {
        Norm(A[0] - A[1]),
        Norm(A[1] - A[2]),
        Norm(B[0] - B[1]),
        Norm(B[2] - B[3]),
    }

    for val in &axes {
        P : [4]f32;
    
        for i in 0..<len(A) { P[i] = Dot(A[i], val)}
        minA, maxA,_ := slc.min_max(P[:])
    
        for i in 0..<len(B) { P[i] = Dot(B[i], val)}
        minB,maxB,_ := slc.min_max(P[:])
      
        if minB > maxA || minA > maxB { return false }
    }

    return true
}

IsCollidingElements :: proc(posA, posB, sizeA, sizeB : Vec2, angleA, angleB : f32) -> bool{
    return IsCollidingBase(
        sdl.Rect{i32(posA.x), i32(posA.y), i32(sizeA.x), i32(sizeA.y)},
        sdl.Rect{i32(posB.x), i32(posB.y), i32(sizeB.x), i32(sizeB.y)},
        angleA,
        angleB,
    )
}

IsColliding :: proc{
    IsCollidingBase,
    IsCollidingElements,
}