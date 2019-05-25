#pragma once

#include <memory>
#include <vector>

#include <SDL2/SDL.h>

#include <ecs/ECS.h>
#include <game/Tiles.h>

class RenderInterface;


struct PassibilityData
{
    int passibility;
};

struct VisibilityData
{
    bool visible;
};

struct LightingData
{
    int lighting;
};



struct ImmutableLevelData
{
    ImmutableLevelData() = default;
    ~ImmutableLevelData() = default;

    int tilePixelSize = -1;

    Vector2i levelSize;
    int tileCount = -1;

    int defaultPassibility = 0;
    bool defaultVisibility = false;
    int defaultLightLevel = 0;

    TileMap tileMap;
    std::vector<TileRef> mapLayout;
};

class LevelContext
{
};

using LevelContextPtr = std::shared_ptr<LevelContext>;

class Level
{
public:

    explicit Level(ImmutableLevelData&& imd, LevelContextPtr ctx);
    virtual ~Level() = default;

    bool input(SDL_Event &evt);
    void update(uint32_t ticks, RenderInterface &rInter);

private:

    void render(uint32_t ticks, RenderInterface &rInter);
    void renderTiles(uint32_t ticks, RenderInterface &rInter);

private:

    ECS m_ecs;
    const ImmutableLevelData m_imData;
    LevelContextPtr m_ctx;

    std::vector<PassibilityData> m_passibilityData;
    std::vector<LightingData> m_lightingData;
    std::vector<VisibilityData> m_visibilityData;

};
using LevelPtr = std::unique_ptr<Level>;
