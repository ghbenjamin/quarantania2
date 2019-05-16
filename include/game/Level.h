#pragma once

#include <memory>
#include <SDL2/SDL.h>

class RenderInterface;

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

private:
};


using LevelPtr = std::unique_ptr<Level>;
using LevelContextPtr = std::unique_ptr<LevelContext>;