package main

import eng "../engine"
import sdl "vendor:sdl2"
import fmt "core:fmt"
import math "core:math"

Penguin :: struct {
    body, gun : eng.AnimatedSprite,
    pos, vel: eng.Vec2,
    linvel, hp: f32,
}

PENGUINROTATE :: math.π
DRAG :: 1
ATRITO :: 300
BREAKS :: 150
ROTATELOSE :: 375/math.TAU
ROTATEANGLE :: 1.0
PENGUINACC :: eng.Vec2{500, 0}
PENGUINMAXVEL :: 625
PBULLETSPEED :: 250

CreatePenguin :: proc(pos : eng.Vec2) -> Penguin {
    using eng
    bd := LoadAnimatedSprite("./resources/img/penguin.png")
    gn := LoadAnimatedSprite("./resources/img/cubngun.png")
    return Penguin{bd, gn, pos + SpriteCenter(&bd.spr), Vec2{}, 0, 45}
}

MovePenguin :: proc(penguin : ^Penguin, dt : f32, layers: ^Layers) {
    using penguin, eng, sdl.Scancode
    
    pos += vel*dt
    pos.x = max(pos.x, layers.pos[0].x)
    pos.x = min(pos.x, layers.pos[0].x + 1408 - f32(body.spr.w))
    pos.y = max(pos.y, layers.pos[0].y)
    pos.y = min(pos.y, layers.pos[0].y + 1280 - f32(body.spr.h))

    lastAngle := body.angle
    
    if IsKeyDown(.A) {
        body.angle -= PENGUINROTATE*dt
    }
    if IsKeyDown(.D) {
        body.angle += PENGUINROTATE*dt
    }
    
    AssertAngle(&body.angle)
    // if lastAngle != body.angle && Mag(vel) >0{
    //     rf := ROTATELOSE*(lastAngle - body.angle)
    //     rot : f32 = 1 if lastAngle < body.angle else -1
    //     vel = Rotate(vel - rf, rot*ROTATEANGLE*(Mag(rf)/Mag(vel)))
    // }

    if IsKeyDown(.W) {
        vel = vel + Rotate(PENGUINACC, body.angle)*dt
    }
    else if Mag(vel) > 0 {
        atrito := Rotate(Vec2{ATRITO,0}, Theta(vel))*dt
        if Mag(vel) < Mag(atrito) { vel*= 0 }
        else { vel -= atrito }
    }
    
    if IsKeyDown(.S) {
        bbreak := Rotate(Vec2{BREAKS, 0}, Theta(vel))*dt
        if Mag(vel) < Mag(bbreak) { vel *= 0 }
        else { vel -= bbreak}
    }
    if Mag(vel) > PENGUINMAXVEL {
        vel = Norm(vel)*PENGUINMAXVEL
    }
    
    gun.angle = Theta((Vec2{f32(input.mouse.x),f32(input.mouse.y)} - pos - SpriteCenter(&gun.spr)) + __camera.pos)
    
    if MousePress(sdl.BUTTON_LEFT) {
        anim := LoadAnimatedSprite("./resources/img/penguinbullet.png", gun.angle, 4, 5*FRAME)
        bulpos := pos + SpriteCenter(&gun.spr) + Rotate(Vec2{f32(gun.spr.w)/2, 0}, gun.angle)
        append(&world1.bullets, CreateBullet(anim, bulpos , PBULLETSPEED, PBULLETSPEED*3, 20))
    }
}

RenderPenguin :: proc(penguin: ^Penguin) {
    using penguin, eng
    RenderAnimated(&body, pos)
    RenderAnimated(&gun, pos)
}

PenguinCollisions :: proc(player: ^Penguin, bullets: ^[dynamic]eng.Bullet) {
    using eng
    for i := 0; i < len(bullets^); i+=1 {
        if bullets[i].owner != 0 && 
            IsColliding(player.pos, bullets[i].pos,
                SpriteDimensions(&player.body),SpriteDimensions(&bullets[i].spr),
                player.body.angle,bullets[i].spr.angle) {
            
            player.hp -= bullets[i].damage
            fmt.println(player.hp)
            unordered_remove(bullets, i)
            i-=1
            
            if player.hp <= 0 {
                ChangeWorldState(2)
                return
            }
        }
    }
}