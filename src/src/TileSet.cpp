#pragma once

#include "../include/TileSet.hpp"
#include <string>

TileSet::TileSet(int tileWidth, int tileHeight, const std::string& file)
{
    this->tileWidth = tileWidth;
    this->tileHeight = tileHeight;
    
    this->tileSet.Open(file);
    this->rows = this->tileSet.GetHeight()/this->tileHeight;
    this->columns = this->tileSet.GetWidth()/this->tileWidth;
}

void TileSet::RenderTile(unsigned index, float x, float y)
{
    if(index >= 0 && index < this->GetTileNumber())
    {
        const int tileWN = this->columns;
        const int tileHN = this->rows;

        const int wIndex = index%tileWN;
        const int hIndex = index/tileWN;
        
        this->tileSet.SetClip(
            wIndex*this->tileWidth,
            hIndex*this->tileHeight,
            this->tileWidth,
            this->tileHeight       
        );

        this->tileSet.Render(x,y);
    }
}

int TileSet::GetTileWidth() { return this->tileWidth; }
int TileSet::GetTileHeight() { return this->tileHeight; }
int TileSet::GetTileNumber() { return this->tileWidth*this->tileHeight; }
int TileSet::GetRows() { return this->rows; }
int TileSet::GetColumns() { return this->columns; }