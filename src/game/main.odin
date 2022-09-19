package main

import "core:fmt"
import eng "../engine"
import sdl "vendor:sdl2"

main :: proc() {
    eng.SetFps(60)
    eng.LoadStates({
        {UpdateStage1,LoadStage1,nil},
        {UpdateStage1,LoadPlayerDeath,nil},
        {UpdateStage1,LoadPlayerWin,nil},
        {UpdateTitle, LoadTitle, DestructTitle},
        {UpdateTransition, LoadTransition, DestructTransition}
    })
    eng.SetWorldState(4)
    eng.GameLoop()
}