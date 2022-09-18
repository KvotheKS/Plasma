package main 

import eng "../engine"
import QU "core:container/queue"
import "core:fmt"

Move :: struct { finalpos : eng.Vec2, }
Shoot :: struct { finalpos : ^eng.Vec2, offset : eng.Vec2,}
Rest :: struct { cooldown, ran : f32,}

Actions :: union{Move,Shoot,Rest}

Alien :: struct {
    anim : eng.AnimatedSprite,
    pos,speed : eng.Vec2,
    actqu : QU.Queue(Actions),
    hp : f32,
    minions: [dynamic]Minion
}

ALBULLETSPEED :: 350
ALBULLETDISTANCE :: 1200
ALIENMAXDIST :: 350
ALIENSPEED :: 350
MOVEREST :: 75*eng.FRAME
SHOOTREST:: 3*eng.FRAME

CreateAlien :: proc(center: eng.Vec2) -> Alien{
    using eng
    anim := LoadAnimatedSprite("./resources/img/alien.png")
    return Alien{anim, center - SpriteCenter(&anim.spr),Vec2{350,0}, {}, 40,CreateMinions(5)}
}

AlienIA :: proc(alien: ^Alien, player: ^Penguin, bullets: ^[dynamic]eng.Bullet) {
    using eng, alien
    dt := DeltaTime()
    if actqu.len > 0 { PopAction(alien, bullets) }
    else {
        aliencenter := pos+SpriteCenter(&anim.spr)
        penguincenter := player.pos+SpriteCenter(&player.body.spr)
        alienmov := Vec2{
            min(Dist(aliencenter, penguincenter), ALIENMAXDIST), 
            0,
        }
        alienmov = Rotate(alienmov, Theta(penguincenter-aliencenter))
        
        QU.push(
            &actqu, 
            Move{pos + alienmov - SpriteCenter(&anim.spr)},
            Rest{SHOOTREST,0},
            Shoot{&player.pos,SpriteCenter(&player.body.spr)},
            Rest{MOVEREST,0},
            
        )
    }
}

Moving :: proc(alien: ^Alien, action: Move) -> bool {
    using eng
    alvel := Rotate(Vec2{ALIENSPEED,0}*DeltaTime(), Theta(action.finalpos-alien.pos))
    if Mag(alvel) > Dist(action.finalpos,alien.pos) { 
        alien.pos = action.finalpos
        return true
    }
    alien.pos += alvel
    return false
}

Shooting :: proc(alien: ^Alien, action: Shoot, bullets: ^[dynamic]eng.Bullet) -> bool {
    using eng
    
    if len(alien.minions) == 0 { return true }
    aliencspr := SpriteCenter(&alien.anim.spr)
    aliencenter := alien.pos + aliencspr
    
    bst : f32 = 0
    cbst : f32 = 10000000
    for val in alien.minions {
        ndst := Dist(aliencenter + Rotate(aliencspr+30,val.angle), action.finalpos^+action.offset)
        if cbst>ndst{
            bst = val.angle
            cbst=ndst
        }
    }
    anim := LoadAnimatedSprite(
        "./resources/img/minionbullet2.png", 
        Theta(action.finalpos^ + action.offset - aliencenter - Rotate(aliencspr+30,bst)), 
        3, 3*FRAME
    )
    bulpos := alien.pos + SpriteCenter(&alien.anim.spr) + Rotate(aliencspr+30,bst)
    append(bullets, CreateBullet(anim, bulpos, ALBULLETSPEED, ALBULLETDISTANCE,15, 1))
    return true
}

Resting :: proc(alien: ^Alien, action: ^Rest) -> bool {
    using eng
    action.ran += DeltaTime()
   
    if action.ran >= action.cooldown { return true }

    return false
}

PopAction :: proc(alien: ^Alien, bullets: ^[dynamic]eng.Bullet) {
    using QU
    if alien.actqu.len > 0 {
        tmp := get_ptr(&alien.actqu,0)
        toPop : bool = false
        
        #partial switch act in tmp {
            case Move:  toPop = Moving(alien, act)
            case Shoot: toPop = Shooting(alien, act, bullets)
            case Rest:  toPop = Resting(alien, &tmp.(Rest))
        }

        if toPop { pop_front(&alien.actqu) }
    }
}

AlienCollision :: proc(
    alien: ^^Alien, bullets: ^[dynamic]eng.Bullet, 
    layers: ^Layers, player: ^Penguin,
    particles: ^[dynamic]eng.SimpleParticle) 
{

    using eng
    aliencspr := SpriteCenter(&alien^.anim.spr)
    aliencenter := alien^.pos + aliencspr

    for i := 0; i < len(bullets^); i+=1 {
        wasHit := false
        for j := 0; j < len(alien^.minions); j+=1 {
            minionpos := aliencenter + Rotate(aliencspr+30,alien^.minions[j].angle) - SpriteCenter(&alien^.minions[j].cpl)
            
            if bullets[i].owner == 0 && 
            
            IsColliding(minionpos, bullets[i].pos,
                SpriteDimensions(&alien^.minions[j].cpl),SpriteDimensions(&bullets[i].spr),
                alien^.minions[j].cpl.angle,bullets[i].spr.angle) 
            {
                
                MinionDeath(aliencenter + Rotate(aliencspr+30,alien^.minions[j].angle), particles)

                unordered_remove(bullets, i)
                unordered_remove(&alien^.minions, j)
                i-=1
                j-=1
                wasHit = true
                break
            }
        }
        
        if !wasHit && bullets[i].owner == 0 && 
            IsColliding(alien^.pos, bullets[i].pos,
                SpriteDimensions(&alien^.anim),SpriteDimensions(&bullets[i].spr),
                alien^.anim.angle,bullets[i].spr.angle) 
        {
            
            alien^.hp -= bullets[i].damage
            unordered_remove(bullets, i)
            i-=1
            
            if alien^.hp <= 0 {
                AlienDeath(alien, particles)
                if player != nil { LoadPlayerWin() }
                return
            }
        }
    }
}

AlienDeath :: proc(alien: ^^Alien, particles: ^[dynamic]eng.SimpleParticle) {
    using eng
    cpl := LoadCompleteSprite("./resources/img/aliendeath.png", {2,2}, 0, 4, FRAME*12)
    ptcpos := alien^.pos + SpriteCenter(&alien^.anim) - SpriteCenter(&cpl)
    snd := CreateSound("./resources/audio/boom.wav")
    append(particles, CreateSimpleParticle(cpl, ptcpos, 0,0, 50*FRAME,snd))

    free(alien^)
    alien^ = nil
}