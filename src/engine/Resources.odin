package engine

import "core:fmt"
import sdl "vendor:sdl2"
import img "vendor:sdl2/image"
import audio "vendor:sdl2/mixer"

@(private)
Resources :: struct {
    textures : map[cstring]^sdl.Texture,
    sounds   : map[cstring]^audio.Chunk,
    songs    : map[cstring]^audio.Music
}

@(private)
rsc : Resources;

GetTexture :: proc(file : cstring)  -> ^sdl.Texture{
    rsc.textures[file] = rsc.textures[file] or_else img.LoadTexture(__ctx.renderer, file)
    return rsc.textures[file]
}

GetSound :: proc(file : cstring) -> ^audio.Chunk{
    rsc.sounds[file] = rsc.sounds[file] or_else audio.LoadWAV(file)
    return rsc.sounds[file]
}

GetSong :: proc(file : cstring) -> ^audio.Music{
    rsc.songs[file] = rsc.songs[file] or_else audio.LoadMUS(file)
    fmt.println(sdl.GetError())
    return rsc.songs[file]
}

ClearTextures :: proc() {
    for _,v in rsc.textures {
        sdl.DestroyTexture(v)
    }
    clear(&rsc.textures)
}

ClearSounds :: proc() {
    for _, v in rsc.sounds {
        audio.FreeChunk(v)
    }
    clear(&rsc.sounds)
}

ClearSongs :: proc() {
    for _,v in rsc.songs {
        audio.FreeMusic(v)
    }
    clear(&rsc.songs)
}

ClearResources :: proc() {
    ClearTextures()
    ClearSounds()
    ClearSongs()
}