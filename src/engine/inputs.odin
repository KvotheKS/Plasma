package engine

import sdl "vendor:sdl2"
import "core:c"

INPUTS :: struct {
    mouse : [2]c.int,
    keyState : [416]bool,
    keyUpdate: [416]c.int,
    mouseState:[6]bool,
    mouseUpdate:[6]c.int,
    quitRequested: bool,
    updateCounter: i32,
    wasPressed : bool,
}

input : INPUTS;

WasKeyPressed :: proc() -> bool { return input.wasPressed }

AnyKeyDown :: proc(keys: []sdl.Scancode) -> bool {
    for val in keys { 
        if IsKeyDown(val) { return true }
    }
    return false
}

KeyPress :: proc(key: sdl.Scancode) -> bool { 
    return input.keyState[key] && (input.keyUpdate[key] == input.updateCounter); 
}

KeyRelease :: proc(key : sdl.Scancode) -> bool { 
    return !input.keyState[key] && (input.keyUpdate[key] == input.updateCounter); 
}
IsKeyDown :: proc(key : sdl.Scancode) -> bool  { 
    return input.keyState[key]; 
}

MousePress :: proc(key : i32) -> bool { 
    return input.mouseState[key] && (input.mouseUpdate[key] == input.updateCounter); 
}

MouseRelease :: proc(key : i32) -> bool  { 
    return !input.mouseState[key] && (input.mouseUpdate[key] == input.updateCounter); 
}

IsMouseDown :: proc(key : i32) -> bool { 
    return input.mouseState[key]; 
}

HearInputs :: proc() {
    event : sdl.Event

    sdl.GetMouseState(&input.mouse.x, &input.mouse.y)
    input.quitRequested = false
    input.updateCounter += 1;
    input.wasPressed = false
    for(sdl.PollEvent(&event)) {
        #partial switch(event.type) {
            case .KEYDOWN:
                if(event.key.repeat != 1) {
                    input.keyState[event.key.keysym.scancode] = true;
                    input.keyUpdate[event.key.keysym.scancode]= input.updateCounter;
                    input.wasPressed = true
                }
            
            case .KEYUP:
                
                input.keyState[event.key.keysym.scancode] = false;
                input.keyUpdate[event.key.keysym.scancode] = input.updateCounter;
                input.wasPressed = true

            case .MOUSEBUTTONDOWN:
                input.mouseState[event.button.button] = true;
                input.mouseUpdate[event.button.button]= input.updateCounter;
                input.wasPressed = true

            case .MOUSEBUTTONUP:
                input.mouseState[event.button.button] = false;
                input.mouseUpdate[event.button.button]= input.updateCounter;
                input.wasPressed = true
            
            case .QUIT:
                input.quitRequested = true;
        }
    }
}