package engine

import sdl "vendor:sdl2"
import img "vendor:sdl2/image"
import math "core:math"

Sprite :: struct {
    texture: ^sdl.Texture,
    w,h : i32,
    clip : sdl.Rect
}

AnimatedSprite :: struct {
    spr : Sprite,
    angle : f32,
    frameCount, currFrame : i32,
    frameTime, currFTime  : f32,
}

CompleteSprite :: struct {
    spr : Sprite,
    size : Vec2,
    angle : f32,
    frameCount, currFrame : i32,
    frameTime, currFTime  : f32,
}

LoadAnimatedSprite :: proc(file : cstring, angle : f32 = 0.0, frameCount : i32 = 1, frameTime : f32 = 1000) -> AnimatedSprite {
    spr := LoadSprite(file)
    SetClip(&spr, 0,0,spr.w/frameCount, spr.h)
    return AnimatedSprite{spr, angle, frameCount, 0, frameTime, 0.0}
}

LoadCompleteSprite :: proc(file : cstring, size : Vec2={1,1}, angle : f32 = 0.0, frameCount : i32 = 1, frameTime : f32 = 1000) -> CompleteSprite {
    spr := LoadSprite(file)
    SetClip(&spr, 0,0,spr.w/frameCount, spr.h)
    return CompleteSprite{spr, size, angle, frameCount, 0, frameTime, 0.0}
}

LoadSprite :: proc(file: cstring) -> Sprite {
    texture := GetTexture(file)
    w,h : i32;
    sdl.QueryTexture(texture, nil, nil, &w, &h)
    return Sprite{texture,w,h, sdl.Rect{0,0,w,h}}
}

SetClip :: proc(spr : ^Sprite, x,y,w,h : i32) {
    spr.clip = sdl.Rect{
        x,y,w,h
    }
}

SpriteWidth_c :: proc(spr: ^Sprite) -> i32{
    return spr.clip.w
}

SpriteHeight_c :: proc(spr: ^Sprite) -> i32{
    return spr.clip.h
}

SpriteWidth_a :: proc(anim: ^AnimatedSprite) -> i32 {
    return SpriteWidth_c(&anim.spr)
}

SpriteHeight_a :: proc(anim: ^AnimatedSprite) -> i32 {
    return SpriteHeight_c(&anim.spr)
}

SpriteWidth_s :: proc(cpl: ^CompleteSprite) -> i32 {
    return i32(cpl.size.x*f32(SpriteWidth_c(&cpl.spr)))
}

SpriteHeight_s :: proc(cpl: ^CompleteSprite) -> i32 {
    return i32(cpl.size.y*f32(SpriteHeight_c(&cpl.spr)))
}

SpriteWidth :: proc{
    SpriteWidth_a,
    SpriteWidth_c,
    SpriteWidth_s,
}

SpriteHeight :: proc{
    SpriteHeight_a,
    SpriteHeight_c,
    SpriteHeight_s,
}

SpriteCenter_c :: proc(spr : ^Sprite) -> Vec2{
    return Vec2{f32(spr.clip.w/2),f32(spr.clip.h/2)}
}

SpriteCenter_a :: proc(anim: ^AnimatedSprite) -> Vec2{
    return SpriteCenter_c(&anim.spr)
}

SpriteCenter_s :: proc(cpl: ^CompleteSprite) -> Vec2{
    return cpl.size*SpriteCenter_c(&cpl.spr)
}

SpriteCenter :: proc{
    SpriteCenter_c,
    SpriteCenter_a,
    SpriteCenter_s,
}

SpriteDimensions_c :: proc(spr : ^Sprite) -> Vec2{
    return Vec2{f32(spr.clip.w/2),f32(spr.clip.h/2)}*2
}

SpriteDimensions_a :: proc(anim: ^AnimatedSprite) -> Vec2{
    return SpriteDimensions_c(&anim.spr)
}

SpriteDimensions_s :: proc(cpl: ^CompleteSprite) -> Vec2{
    return cpl.size*SpriteDimensions_c(&cpl.spr)
}

SpriteDimensions :: proc{
    SpriteDimensions_c,
    SpriteDimensions_a,
    SpriteDimensions_s,
}

PosFromCenter_c :: proc(spr : ^Sprite, pos: Vec2) -> Vec2{
    return Vec2{pos.x - f32(spr.clip.w/2), pos.y - f32(spr.clip.h/2)}
}

PosFromCenter_s :: proc(cpl: ^CompleteSprite, pos: Vec2) -> Vec2{
    return pos - SpriteCenter(cpl)
}

PosFromCenter :: proc{
    PosFromCenter_c,
    PosFromCenter_s,
}

RenderSprite :: proc(spr : ^Sprite, dst : Vec2) {
    sdl.RenderCopy(__ctx.renderer, spr.texture, &spr.clip, &sdl.Rect{i32(dst.x), i32(dst.y), spr.clip.w, spr.clip.h})
}

RenderAnimated :: proc(anim: ^AnimatedSprite, pos: Vec2, center : ^sdl.Point = nil) {
    using anim
    dt := DeltaTime()

    currFTime += dt
    if currFTime > frameTime {
        currFTime -= frameTime
        currFrame = (currFrame+1)%frameCount
        spr.clip.x = spr.clip.w*currFrame
    }

    RenderSpriteEx(&spr, pos - __camera.pos, f64(angle))
}

RenderAnimated_S :: proc(anim: ^AnimatedSprite, pos: Vec2, center : ^sdl.Point = nil) {
    using anim
    dt := DeltaTime()

    currFTime += dt
    if currFrame < frameCount - 1 && currFTime > frameTime {
        currFTime -= frameTime
        currFrame = (currFrame+1)%frameCount
        spr.clip.x = spr.clip.w*currFrame
    }

    RenderSpriteEx(&spr, pos - __camera.pos, f64(angle))
}

RenderComplete :: proc(cpl: ^CompleteSprite, pos: Vec2) {
    using cpl
    dt := DeltaTime()

    currFTime += dt
    if currFrame < frameCount - 1 && currFTime > frameTime {
        currFTime -= frameTime
        currFrame = (currFrame+1)%frameCount
        spr.clip.x = spr.clip.w*currFrame
    }

    sdl.RenderCopyEx(
        __ctx.renderer,
        spr.texture,
        &spr.clip,
        &sdl.Rect{i32(pos.x-__camera.pos.x), i32(pos.y-__camera.pos.y), SpriteWidth(cpl), SpriteHeight(cpl)},
        f64(angle)*(180/math.π), 
        nil, 
        sdl.RendererFlip.NONE,
    )
}

Render :: proc{
    RenderAnimated,
    RenderSprite,
    RenderComplete,
}

RenderWindowSprite :: proc(spr: ^Sprite) {
    sdl.RenderCopy(__ctx.renderer, spr.texture, &spr.clip, &sdl.Rect{0, 0, i32(__ctx.size.x), i32(__ctx.size.y)})
}

RenderSpriteEx :: proc(spr : ^Sprite, dst : Vec2, angle : f64, center : ^sdl.Point = nil) {
    sdl.RenderCopyEx(
        __ctx.renderer, 
        spr.texture, 
        &spr.clip, 
        &sdl.Rect{i32(dst.x), i32(dst.y), spr.clip.w, spr.clip.h}, 
        angle*(180/math.π), 
        center, 
        sdl.RendererFlip.NONE
    )
}