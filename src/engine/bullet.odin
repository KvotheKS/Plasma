package engine

Bullet :: struct {
    spr : AnimatedSprite,
    pos : Vec2,
    speed, limit : f32,
    totaldist, damage: f32,
    owner : i32,
}

CreateBullet :: proc(anim : AnimatedSprite, pos : Vec2, speed,limit, damage : f32, owner : i32 = 0) -> Bullet {
    banim := anim
    return Bullet{
        banim, 
        pos + Rotate(Vec2{f32(SpriteWidth(&banim.spr))/2,0},anim.angle) - SpriteCenter(&banim.spr), 
        speed, 
        limit,
        0,
        damage,
        owner
    }
}

UpdateBullet :: proc(bullet : ^Bullet) -> bool {
    using bullet
    dt := DeltaTime() 
    disloc := Rotate(Vec2{speed, 0}*dt, spr.angle)
    pos += disloc
    totaldist += speed*dt
    return totaldist < limit
}

UpdateBullets :: proc(bullets: ^[dynamic]Bullet) {
    for i := 0; i < len(bullets^); i+=1 {
        if !UpdateBullet(&bullets[i]) {
            unordered_remove(bullets, i)
            i-=1
        }
    }
}