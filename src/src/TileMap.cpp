#pragma once

#include "../include/Component.hpp"
#include "../include/GameObject.hpp"
#include "../include/TileMap.hpp"
#include "../include/Game.hpp"
#include "../include/Camera.hpp"
#include <string>
#include <cstdlib>
#include <cmath>

TileMap::TileMap() : Component() 
{ this->tileSet = nullptr; }

TileMap::TileMap(GameObject& associated, 
            const std::string& file, TileSet* tileSet)
    : Component(associated)
{
    Load(file);
    SetTileSet(tileSet);
}
    
TileMap::~TileMap() 
{
    if(this->tileSet)
        delete this->tileSet;
}

void TileMap::SetTileSet(TileSet* tileSet)
{ this->tileSet = tileSet; }

int TileMap::AtIdx(int x, int y, int z)
{ return this->mapWidth*(z*this->mapHeight + y) + x; }

int& TileMap::At(int x, int y, int z)
{ return this->tileMatrix[this->mapWidth*(z*this->mapHeight + y) + x]; }
    
void TileMap::Load(const std::string& file)
{
    FILE* fl = fopen(file.c_str(), "r");
    
    fscanf(fl, "%d,%d,%d,", &this->mapWidth, &this->mapHeight, &this->mapDepth);

    this->tileMatrix.resize(this->mapWidth*this->mapHeight*this->mapDepth);
    
    int tmp;
    const int vSize = this->mapHeight*this->mapWidth*this->mapDepth;
    
    for(int i = 0; i < vSize; i++)
    {
        fscanf(fl, "%d,", &tmp);
        this->tileMatrix[i] = tmp - 1; 
    } 
}

Rect TileMap::adapt(const Rect& tSetC)
{
    const float tW = this->tileSet->GetTileWidth();
    const float tH = this->tileSet->GetTileHeight();
    
    const float xl = floor(tSetC.x/tW)*tW;
    const float yl = floor(tSetC.y/tH)*tH;

    const float wl = ceil((tSetC.x+tSetC.w)/tW)*tW - xl;
    const float hl = ceil((tSetC.y+tSetC.h)/tH)*tH - yl;
    
    return {xl,yl,wl,hl};
}

Rect TileMap::tileCord(Rect& tSetC)
{
    /*
        Retorna as coordenadas do primeiro tile a ser renderizado e tambem
        as do ultimo. Assume que tSetC esta alinhado com o tileset e associated.box
    */
    const float tW = this->tileSet->GetTileWidth();
    const float tH = this->tileSet->GetTileHeight();

    const float xl = tSetC.x/tW;
    const float yl = tSetC.y/tH;

    const float x2l = (tSetC.x + tSetC.w)/tW;
    const float y2l = (tSetC.y + tSetC.h)/tH;

    return {xl,yl,x2l,y2l};
}

void TileMap::Update(float dt) {}

void TileMap::Render()
{
    Game& inst = Game::GetInstance();
    const Vec2 cPos = Camera::pos;
    int cameraX = cPos.x, cameraY = cPos.y;

    Rect gCamera = { (float)cameraX, (float)cameraY, 
                           (float)inst.GetWidth(), (float)inst.GetHeight() };

    if(!this->associated.box.intersect(gCamera))
        return;
    
    Rect toRender = adapt(gCamera.intersection(this->associated.box.copy()));
    Rect tSetC = tileCord(toRender);
    
    toRender.x -= cameraX; toRender.y -= cameraY;
    
    for(int i = 0; i < this->mapDepth; i++)
        RenderLayer(i, toRender, tSetC);
}

void TileMap::RenderLayer(int layer, const Rect& toRender, const Rect& tSetC)
{    
    const int tW = this->tileSet->GetTileWidth();
    const int tH = this->tileSet->GetTileHeight();

    const int fs = AtIdx(tSetC.x, tSetC.y, layer);

    const int nxt = AtIdx(tSetC.x, tSetC.y+1, layer) - AtIdx(tSetC.w, tSetC.y, layer);

    for(int i = fs, y = toRender.y; y < toRender.y + toRender.h; y+=tH, i+=nxt)
        for(int x = toRender.x; x < toRender.x + toRender.w; i++, x+=tW)
        {
            if(this->tileMatrix[i] != -1)
                this->tileSet->RenderTile(this->tileMatrix[i], x, y);   
        }
}

bool TileMap::Is(const std::string& type) 
{ return type == "TileMap"; }

int TileMap::GetWidth() { return this->mapWidth; }
int TileMap::GetHeight() { return this->mapHeight; }
int TileMap::GetDepth() { return this->mapDepth; }
int TileMap::GetPWidth() { return this->mapWidth*tileSet->GetTileWidth(); }
int TileMap::GetPHeight() { return this->mapHeight*tileSet->GetTileHeight(); }