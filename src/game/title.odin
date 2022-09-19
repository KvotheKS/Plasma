package main

import eng "../engine"
import "core:fmt"
import "core:runtime"
import sdl "vendor:sdl2"

TitleScreen_t :: struct{
    text : eng.Text,
    scrn : eng.Sprite,
}

TitleScreen : TitleScreen_t;

UpdateTitle :: proc(dt : f32) -> bool{
    using eng, TitleScreen
    postmp := __camera.size
    RenderWindowSprite(&scrn);
    RenderFont(&text, Vec2{postmp.x*0.6, postmp.y*0.15});
    if AnyKeyDown({.ESCAPE}) {
        return false
    }
    else if WasKeyPressed() {
        ChangeWorldState(1)
    }
    return true
}

LoadTitle :: proc() {
    using eng, TitleScreen
    postmp := __camera.size
    scrn = LoadSprite("./resources/img/title.jpg")
    // fmt.println(scrn)
    text = LoadFont(
        "./resources/font/Call me maybe.ttf", 
        16, 
        Vec2{postmp.x*0.2,postmp.y*0.8},
        .SOLID,
        "Pressione qualquer botao para jogar",
        sdl.Color{127,0, 127, 255}
    )
}

DestructTitle :: proc() {
    using eng
    ClearResources()
}