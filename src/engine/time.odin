package engine

import fmt "core:fmt"
import sdl "vendor:sdl2"

FRAME :: (1.0/60.0)

T_INFO :: struct {
    last     : u32,
    dt       : f32,
    fps      : f32,
}

@(private)
G_T_INFO := T_INFO{0, 0.0, 1.0};

Clock :: proc() {
    lsn := sdl.GetTicks()
    G_T_INFO.dt = f32(lsn - G_T_INFO.last)/1000.0
    G_T_INFO.last = lsn
}

Wait :: proc() {
    lsn := f32(sdl.GetTicks() - G_T_INFO.last)/1000.0

    cfps := u32(max((G_T_INFO.fps - lsn)*1000.0,0.0))
    sdl.Delay(cfps)
}

SetFps :: proc(fps : f32) {
    G_T_INFO.fps = 1.0/fps
    fmt.println(G_T_INFO.fps)
}

DeltaTime :: proc() -> f32 {
    return G_T_INFO.dt;
}