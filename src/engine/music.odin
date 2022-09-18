package engine

import audio "vendor:sdl2/mixer"
import "core:fmt"

Music :: struct {
    music: ^audio.Music,
}

CreateMusic :: proc(path : cstring) -> Music{
    return Music{GetSong(path)}
}

PlayMusic :: proc(music: ^Music, times : i32 = -1) {
    audio.PlayMusic(music.music, times)
}

FadeoutMusic :: proc(timeOut: i32 = 0) {
    audio.FadeOutMusic(timeOut)
}

Stopmusic :: proc() {
    audio.FadeOutMusic(0)
}