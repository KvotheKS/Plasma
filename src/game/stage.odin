package main

import eng "../engine"
import "core:fmt"
import "core:runtime"
import sdl "vendor:sdl2"

Layers :: struct {
    tileMap : eng.Tilemap,
    pos : [dynamic]eng.Vec2,
    depth : [dynamic]f32,
}

World :: struct {
    player : ^Penguin,
    stickyBkg : eng.Sprite,
    layers : Layers,
    bullets : [dynamic]eng.Bullet,
    particles: [dynamic]eng.SimpleParticle,
    enemie  : ^Alien,
}

FinalTransition :: struct {
    lwspr : eng.Sprite,
    timeout, showscreen : f32,
    text : eng.Text,
}

world1 : World;
FinishWorld : FinalTransition;

LoadStage1 :: proc() {
    using world1
    using eng
    stickyBkg = LoadSprite("./resources/img/ocean.jpg")
    layers.tileMap = CreateTilemap(
        "./resources/img/tileset.png", 
        "./resources/map/tileMap.txt",
        64,
        64
    )
    append(&layers.pos, 0, 0)
    append(&layers.depth, 0.75,0)

    player = new(Penguin)
    player^ = CreatePenguin(Vec2{300,300})
    SetCameraFollow(&player.pos, SpriteCenter(&player.body.spr), 0.75)
    enemie = new(Alien)
    enemie^ = CreateAlien(Vec2{0,0})
}

DestructStage1 :: proc() {
    using eng, world1
    if player != nil { free(player) }
    if enemie != nil { free(enemie) }
    clear(&layers.pos)
    clear(&layers.depth)
    clear(&bullets)
    clear(&particles)
}

RenderEveryStage1 :: proc() {
    using world1
    using eng

    RenderWindowSprite(&stickyBkg)
    
    RenderLayer(&layers.tileMap, layers.pos[0], 0)
    
    if player != nil { RenderPenguin(player) }
    
    if enemie != nil { 
        RenderAnimated(&enemie.anim, enemie.pos)
        RenderMinions(enemie, &enemie.minions) 
    }
    for val in &bullets  { RenderAnimated_S(&val.spr, val.pos) }
    for val in &particles{ RenderComplete(&val.cpl, val.pos)}

    RenderLayer(&layers.tileMap, layers.pos[1], 1)

    if FinishWorld.showscreen != 0 {RenderWindowSprite(&FinishWorld.lwspr)}
}

UpdateElementsStage1 :: proc(dt : f32) -> bool {
    using world1
    using eng

    if player != nil {
        MoveCameraFollower()
        for i in 0..<len(layers.pos) { MoveFollowerParallax(&layers.pos[i],layers.depth[i]) }
    }
    else {
        MoveCameraKeys()

        MoveKeysParallax(&enemie.pos, layers.depth[0])
        for i in 0..<len(layers.pos) { MoveKeysParallax(&layers.pos[i],layers.depth[i]) }
        for val in &particles { MoveKeysParallax(&val.pos, layers.depth[0])}
        for val in &bullets { MoveKeysParallax(&val.pos, layers.depth[0])}
    }

    if player == nil || enemie == nil {
        FinishWorld.timeout -= dt
        if FinishWorld.timeout <= 0 { 
            if FinishWorld.timeout > -FinishWorld.showscreen {
                sdl.SetTextureAlphaMod(FinishWorld.lwspr.texture, u8(255.0*(-FinishWorld.timeout/FinishWorld.showscreen)))
            }
            else { ChangeWorldState(5) }
        }
    }
    
    UpdateSimpleParticles(&particles)
    UpdateBullets(&bullets)
    return true
}

UpdateStage1 :: proc(dt : f32) -> bool {
    using world1
    using eng

    if AnyKeyDown({.ESCAPE}) {
        return false
    }
    
    if player != nil {
        MovePenguin(player, dt, &layers)
        if enemie != nil { AlienIA(enemie, player, &bullets, &layers) }
    }

    if !UpdateElementsStage1(dt) { return false }
    
    RenderEveryStage1()
    
    if enemie != nil { AlienCollision(&enemie, &bullets,  player, &particles) }
    if player != nil { PenguinCollisions(player, &bullets) }

    return true
}

LoadPlayerDeath :: proc() {
    using eng, world1
    FinishWorld.lwspr = LoadSprite("./resources/img/lose.jpg")
    sdl.SetTextureBlendMode(FinishWorld.lwspr.texture, sdl.BlendMode.BLEND);
    sdl.SetTextureAlphaMod(FinishWorld.lwspr.texture, 0)
    FinishWorld.timeout = 60*FRAME
    FinishWorld.showscreen = 240*FRAME

    cpl := LoadCompleteSprite("./resources/img/penguindeath.png", {1.5,1.5}, 0, 5, FRAME*5)
    ptcpos := player.pos + SpriteCenter(&player.body) - SpriteCenter(&cpl)
    append(&particles, CreateSimpleParticle(cpl, ptcpos, 0,0, 30*FRAME))

    free(player)
    player = nil
}

LoadPlayerWin :: proc() {
    using eng, world1
    FinishWorld.lwspr = LoadSprite("./resources/img/win.jpg")
    sdl.SetTextureBlendMode(FinishWorld.lwspr.texture, sdl.BlendMode.BLEND);
    sdl.SetTextureAlphaMod(FinishWorld.lwspr.texture, 0)
    FinishWorld.timeout = 60*FRAME
    FinishWorld.showscreen = 240*FRAME
    player.hp = 10000
}

UpdateTransition :: proc(dt : f32) -> bool {
    using eng, world1
    RenderWindowSprite(&FinishWorld.lwspr)
    postmp := __camera.size

    RenderFont(&FinishWorld.text, Vec2{postmp.x*0.6, postmp.y*0.15})
    if AnyKeyDown({.ESCAPE}) {
        return false
    }
    else if WasKeyPressed() {
        ChangeWorldState(1)
    }
    return true
}

LoadTransition :: proc() {
    using eng, FinishWorld, world1
    postmp := __camera.size
    txtcolor := sdl.Color{0,255,0,255}
    if player == nil {
        txtcolor = sdl.Color{255,0,0,255}
    }

    FinishWorld.text = LoadFont(
        "./resources/font/Call me maybe.ttf", 
        16, 
        Vec2{postmp.x*0.2,postmp.y*0.85},
        .SOLID,
        "Pressione qualquer botao para jogar",
        txtcolor
    )
}

DestructTransition :: proc() {
    DestructStage1()
    FinishWorld.showscreen = 0
}