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



class LevelContext
{
};

class Level
{
public:

    Level();
    virtual ~Level() = default;

    bool input(SDL_Event &evt);
    void update(uint32_t ticks, RenderInterface &rInter);

    const Vector2i &getSize() const;
    void setSize(const Vector2i &size);



private:

    Vector2i m_size;
    ECS m_ecs;

    std::vector<PassibilityData> m_passibilityData;
    std::vector<LightingData> m_lightingData;
    std::vector<VisibilityData> m_visibilityData;

};


using LevelPtr = std::unique_ptr<Level>;
using LevelContextPtr = std::unique_ptr<LevelContext>;