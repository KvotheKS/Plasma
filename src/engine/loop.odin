package engine

import sdl "vendor:sdl2"

Updater :: proc(dt : f32) -> bool;
Loader :: proc();

State :: struct {
    upd : Updater,
    ldr, dtr : Loader,
}

States :: struct {
    states: [dynamic]State,
    currState: i32,
}

__WorldStates : States;

LoadState :: proc(el: State) {
    append(&__WorldStates.states, el)
}

LoadStates :: proc(els : []State) {
    for val in els {
        LoadState(val)
    }
}

CallLoader :: proc() {
    if __WorldStates.states[__WorldStates.currState].ldr != nil { 
        __WorldStates.states[__WorldStates.currState].ldr() 
    }
}

CallDestructor :: proc() {
    if __WorldStates.states[__WorldStates.currState].dtr != nil { 
        __WorldStates.states[__WorldStates.currState].dtr() 
    }
}

CallUpdate :: proc(dt : f32) -> bool {
    if __WorldStates.states[__WorldStates.currState].upd == nil { return false }

    return __WorldStates.states[__WorldStates.currState].upd(dt)
}

ChangeWorldState :: proc(idx : i32) {
    CallDestructor()
    __WorldStates.currState = idx-1
    CallLoader()
}

GameLoop :: proc() {
    ctx := InitAll()
    if __WorldStates.currState == 0 { CallLoader() }

    for !sdl.QuitRequested() {
        Clock()
        HearInputs()
        sdl.RenderClear(ctx.renderer)
        
        if !CallUpdate(DeltaTime()) {
            break;
        }

        sdl.RenderPresent(ctx.renderer)
        Wait()
    }
    ClearResources()
    DestroyContext()    
    CallDestructor()
}