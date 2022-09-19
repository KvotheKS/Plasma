package main

import eng "../engine"
import rnd "core:math/rand"
import "core:fmt"
import "core:math"

MINIONSPEED :: math.PI/6

Minion :: struct {
    angle : f32, //posicao ao redor do alien
    cpl  : eng.CompleteSprite,
}

CreateMinions :: proc(quantity : i32) -> [dynamic]Minion{
    using eng
    troops : [dynamic]Minion
    stp := math.TAU/f32(quantity)
    for i in 0..<quantity {
        x   := rnd.float32_range(0.5,1.5)
        tmp := LoadCompleteSprite("./resources/img/minion.png", Vec2{1,1}*x)
        append(&troops, Minion{stp*f32(i),tmp})
    }
    return troops
}

RenderMinions :: proc(alien: ^Alien, minions : ^[dynamic]Minion) {
    using eng
    dt := DeltaTime()
    for i in 0..<len(minions^) {
        aliencenter := SpriteCenter(&alien.anim.spr)
        tmp := alien.pos + aliencenter + Rotate(aliencenter+30,minions[i].angle)
        RenderComplete(&minions[i].cpl, PosFromCenter(&minions[i].cpl, tmp))
        minions[i].angle += MINIONSPEED*dt
        AssertAngle(&minions[i].angle)
    }
}

MinionDeath :: proc(center : eng.Vec2, particles: ^[dynamic]eng.SimpleParticle) {
    using eng
    cpl := LoadCompleteSprite("./resources/img/aliendeath.png", {0.5,0.5}, 0, 4, FRAME*5)
    ptcpos := center - SpriteCenter(&cpl)
    append(particles, CreateSimpleParticle(cpl, ptcpos, 0,0, 25*FRAME))
}