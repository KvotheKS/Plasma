package main

import "core:fmt"
import eng "../engine"
import sdl "vendor:sdl2"

main :: proc() {
    eng.SetFps(60)
    eng.LoadStates({
        {UpdateStage1,LoadStage1,DestructStage1},
    })
    eng.GameLoop()
}