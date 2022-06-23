#pragma once

#include "./decl.hpp"
#include "./Sprite.hpp"
#include <string>

class TileSet
{
private:
    Sprite tileSet;
    int rows, columns;
    int tileWidth, tileHeight;

public:
    TileSet(int tileWidth, int tileHeight, const std::string& file);
    void RenderTile(unsigned index, float x, float y);

    int GetTileWidth();
    int GetTileHeight();
    int GetTileNumber();
    int GetRows();
    int GetColumns();
};