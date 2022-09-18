package engine

import audio "vendor:sdl2/mixer"

Sound :: struct {
    sound : ^audio.Chunk,
    channel : i32,
}

CreateSound :: proc(path : cstring) -> Sound{
    return Sound{GetSound(path), -1}
}

PlaySound :: proc(sound: ^Sound, times : i32 = 0) {
    if sound.channel == -1 || audio.Playing(sound.channel) == 0 {
        sound.channel = audio.PlayChannel(-1, sound.sound, times)
    }
}

IsSoundPlaying :: proc(sound: ^Sound) ->bool{
    if sound.channel == -1 {return false}
    if audio.Playing(sound.channel) == 0 { return true }
    
    sound.channel = -1
    
    return false
}

StopSound :: proc(sound: ^Sound) {
    if sound.channel != -1 {
        audio.HaltChannel(sound.channel)
    }
    sound.channel = -1
}