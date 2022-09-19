package engine

import "core:fmt"
import sdl "vendor:sdl2"
import img "vendor:sdl2/image"
import ttf "vendor:sdl2/ttf"
import audio "vendor:sdl2/mixer"

Context :: struct {
    window: ^sdl.Window,
    renderer: ^sdl.Renderer,
    size : Vec2
}

__ctx : Context;

InitAll :: proc() -> Context {
    sdl.Init(sdl.INIT_VIDEO | sdl.INIT_AUDIO | sdl.INIT_TIMER)
    img.Init(img.INIT_JPG | img.INIT_PNG | img.INIT_TIF)
    audio.Init(audio.INIT_FLAC | audio.INIT_OGG | audio.INIT_MP3)
    audio.AllocateChannels(32)
    ttf.Init()

    window := sdl.CreateWindow(
        "Penguin War Crimes", 
        sdl.WINDOWPOS_CENTERED, 
        sdl.WINDOWPOS_CENTERED, 
        640, 
        480, 
        sdl.WINDOW_SHOWN | sdl.WINDOW_RESIZABLE
    ) 

    renderer := sdl.CreateRenderer(
        window,
        -1,
        sdl.RENDERER_ACCELERATED
    )
    w,h : i32
    
    sdl.GetWindowSize(window, &w, &h)
    
    __ctx = Context{window, renderer, Vec2{f32(w),f32(h)}};
    
    InitializeCamera()

    return __ctx;
}

DestroyContext :: proc() {
    sdl.DestroyWindow(__ctx.window)
    sdl.DestroyRenderer(__ctx.renderer)
    img.Quit();
    audio.Quit();
    ttf.Quit();
    sdl.Quit();
}