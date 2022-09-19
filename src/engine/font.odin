package engine

import sdl "vendor:sdl2"
import ttf "vendor:sdl2/ttf"
import "core:fmt"

TextStyle :: enum{SOLID, SHADED, BLENDED}

Text :: struct {
    font : ^ttf.Font,
    pos : Vec2,
    texture : ^sdl.Texture,
    text : cstring,
    style : TextStyle,
    fontFile : cstring,
    fontSize : i32,
    fg,bg : sdl.Color,
}

LoadFont :: proc(fontFile : cstring, fontSize : i32, pos : Vec2, style : TextStyle, text : cstring, fg : sdl.Color, bg : sdl.Color = {0,0,0,0}) -> Text{
    using ttf
    txt := Text{OpenFont(fontFile,fontSize), pos,nil,text,style,fontFile,fontSize,fg,bg};
    RemakeTexture(&txt)
    return txt
}

RemakeTexture :: proc(txt : ^Text) {
    if txt.texture != nil { sdl.DestroyTexture(txt.texture); txt.texture = nil}
    sfc : ^sdl.Surface;

    switch txt.style {
        case .BLENDED : sfc = ttf.RenderText_Blended(txt.font, txt.text, txt.fg);
        case .SOLID : sfc = ttf.RenderText_Solid(txt.font, txt.text, txt.fg)
        case .SHADED : sfc = ttf.RenderText_Shaded(txt.font, txt.text, txt.fg, txt.bg)
    }
    txt.texture = sdl.CreateTextureFromSurface(__ctx.renderer, sfc)
    sdl.FreeSurface(sfc)
}

RenderFont :: proc(txt : ^Text, size : Vec2) {
    if txt.texture != nil {
        sdl.RenderCopy(
            __ctx.renderer, 
            txt.texture, 
            &sdl.Rect{0,0,i32(size.x),i32(size.y)}, 
            &sdl.Rect{i32(txt.pos.x), i32(txt.pos.y), i32(size.x),i32(size.y)}, 
        )
    }
}

FreeFont :: proc(txt : ^Text) {
    sdl.DestroyTexture(txt.texture)
}