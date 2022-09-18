package engine

import libc "core:c/libc"
import fmt "core:fmt"
import sdl "vendor:sdl2"
import img "vendor:sdl2/image"

Tileset :: struct {
    spr : Sprite,
    w, h : i32
}

Tilemap :: struct {
    tset : Tileset,
    w,h,d : i32,
    data  : [dynamic]i16
}

CreateTileset :: proc(path : cstring, w,h : i32) -> Tileset {
    return Tileset{LoadSprite(path), w, h}
}

IdxMap :: proc(tmap : ^Tilemap, x, y: i32, z : i32 = 0) -> i32 {
    return tmap.w*(z*tmap.h + y) + x
}

AtIdx :: proc(tmap : ^Tilemap,x, y: i32, z : i32 = 0) -> ^i16 {
    return &tmap.data[IdxMap(tmap,x,y,z)]
}

TilemapWidth :: proc(tmap : ^Tilemap) -> i32 {
    return tmap.w*tmap.tset.w
}

TilemapHeight :: proc(tmap : ^Tilemap) -> i32 {
    return tmap.h*tmap.tset.h
}

TilemapDst :: proc(tmap : ^Tilemap, pos : Vec2) -> sdl.Rect {
    return sdl.Rect{
        i32(pos.x),
        i32(pos.y),
        TilemapWidth(tmap),
        TilemapHeight(tmap),
    }
}

CreateTilemap :: proc(tset, tmap : cstring, w, h : i32) -> Tilemap {
    ts := CreateTileset(tset,w,h)
    f := libc.fopen(tmap, "r")

    if f == nil {
        fmt.println("Tilemap file does not exist\n\n")
    }
    
    w,h,d: i32
    tmp : i16
    libc.fscanf(f, "%d, %d, %d,", &w, &h, &d)
    mp := Tilemap{ts, w, h, d, {}}
    for i in 0..<w*h*d {
        libc.fscanf(f, "%hd,", &tmp)
        append(&mp.data, tmp-1)
    }
    return mp
}

RenderTile :: proc(tset : ^Tileset, id : i16, x, y : i32) {
    tr  := i16(tset.spr.w/tset.w)
    xid := tset.w*i32(id%tr)
    yid := tset.h*i32(id/tr)
    SetClip(&tset.spr, xid,yid,tset.w,tset.h)
    RenderSprite(&tset.spr, Vec2{f32(x),f32(y)})
}

RenderLayer :: proc(tmap : ^Tilemap, pos : Vec2, depth : i32) -> bool {
    lf := sdl.Rect{
        0,
        0,
        TilemapWidth(tmap),
        TilemapHeight(tmap),
    }

    dst := sdl.Rect{
        i32(pos.x),
        i32(pos.y),
        lf.w,
        lf.h,
    }

    ins := ObjectInCamera(dst) or_return

    idxs := sdl.Rect{
        (ins.x-dst.x)/tmap.tset.w,
        (ins.y-dst.y)/tmap.tset.h,
        i32(ceil(f32(ins.w)/f32(tmap.tset.w))) + i32((ins.x-dst.x)%tmap.tset.w!=0),
        i32(ceil(f32(ins.h)/f32(tmap.tset.h))) + i32((ins.y-dst.y)%tmap.tset.h!=0),
    }

    lf.x = dst.x + idxs.x*tmap.tset.w - i32(__camera.pos.x)
    lf.y = dst.y + idxs.y*tmap.tset.h - i32(__camera.pos.y)
    
    
    y := lf.y
    for i in idxs.y..<min(idxs.h+idxs.y,tmap.h) {
        cn := IdxMap(tmap, idxs.x, i, depth)
        x := lf.x
        for j in idxs.x..<min(idxs.w+idxs.x,tmap.w) {
            if tmap.data[cn] != -1 {
                RenderTile(&tmap.tset, tmap.data[cn], x,y)
            }
            x += tmap.tset.w
            cn += 1
        }
        y += tmap.tset.h
    }
    return true
}

RenderTilemap :: proc(tmap : ^Tilemap, pos : Vec2) {
    for z in 0..<tmap.d {
        RenderLayer(tmap, pos, z)
    }
}