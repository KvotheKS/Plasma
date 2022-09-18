package engine

SimpleParticle :: struct {
    cpl : CompleteSprite,
    pos, speed : Vec2,
    timeout, currtime : f32,
    sound: Sound,
    times: i32,
}

CreateSimpleParticle :: proc(cpl: CompleteSprite, pos : Vec2, speed, angle, timeout : f32, sound: Sound = {}, times:i32=0) -> SimpleParticle{
    smpt := SimpleParticle{cpl, pos, Rotate(Vec2{speed,0},angle), timeout, 0, sound,times}
    PlaySound(&smpt.sound, times)    
    
    return smpt
}

UpdateSimpleParticles :: proc(sprt : ^[dynamic]SimpleParticle){
    dt := DeltaTime()
    vSize := len(sprt^)
    for i := 0; i < vSize; i+=1 {
        sprt[i].currtime += dt
        sprt[i].pos += sprt[i].speed*dt
        if sprt[i].currtime >= sprt[i].timeout { 
            unordered_remove(sprt, i)
            i -= 1
            vSize -= 1
        }
    }
}