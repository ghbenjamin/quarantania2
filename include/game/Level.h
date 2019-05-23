#pragma once

#include <memory>
#include <vector>

#include <SDL2/SDL.h>
#include <ecs/ECS.h>

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


struct LevelInitData
{
    LevelInitData() = default;
    ~LevelInitData() = default;

    Vector2i levelSize;

    int defaultPassibility = 0;
    bool defaultVisibility = false;
    int defaultLightLevel = 0;
};

class LevelContext
{
};

using LevelContextPtr = std::unique_ptr<LevelContext>;

class Level
{
public:

    explicit Level(LevelInitData const& ctx);
    virtual ~Level() = default;

    bool input(SDL_Event &evt);
    void update(uint32_t ticks, RenderInterface &rInter);

    const Vector2i &getSize() const;

private:

    Vector2i m_size;
    int m_tileCount;
    ECS m_ecs;

    std::vector<PassibilityData> m_passibilityData;
    std::vector<LightingData> m_lightingData;
    std::vector<VisibilityData> m_visibilityData;

};
using LevelPtr = std::unique_ptr<Level>;
