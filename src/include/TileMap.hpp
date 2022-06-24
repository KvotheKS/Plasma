#pragma once

#include "./decl.hpp"
#include "./Component.hpp"
#include "./TileSet.hpp"
#include "./GameObject.hpp"
#include "./Rect.hpp"
#include <string>
#include <vector>

class TileMap : public Component
{
private:
    std::vector<int> tileMatrix;
    TileSet* tileSet;
    int mapWidth, mapHeight, mapDepth;

public:
    TileMap();
    TileMap(GameObject& associated, const std::string& file, TileSet* tileSet);
    
    ~TileMap();

public:    
    void Load(const std::string& file);
    void SetTileSet(TileSet* tileSet);

    int AtIdx(int x, int y, int z = 0);
    
    int& At(int x, int y, int z = 0);

public:
    void Render();
    void RenderLayer(int layer, const Rect& toRender, const Rect& tSetC);
    void Update(float dt);

public:
    bool Is(const std::string& type);

public:
    int GetWidth();
    int GetHeight();
    int GetDepth();
    int GetPWidth();
    int GetPHeight();

public:
    Rect adapt(const Rect& tSetC);
    Rect tileCord(Rect& tSetC);
};
