#pragma once

#include <memory>
#include <SDL.h>

class RenderInterface;

class GameState
{
public:

    explicit GameState() = default;
    virtual ~GameState() = default;

    virtual bool input(SDL_Event& evt) = 0;
    virtual void update(Uint32 ticks, RenderInterface&) = 0;
};

using GameStatePtr = std::unique_ptr<GameState>;

