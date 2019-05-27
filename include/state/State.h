#pragma once

#include <memory>

class RenderInterface;
class InputInterface;
struct IEvent;

class GameState
{
public:

    explicit GameState() = default;
    virtual ~GameState() = default;

    virtual bool input(IEvent& evt) = 0;
    virtual void update(uint32_t ticks, InputInterface&, RenderInterface&) = 0;
};

using GameStatePtr = std::unique_ptr<GameState>;

