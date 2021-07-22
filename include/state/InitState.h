#pragma once

#include <state/GameState.h>

class RunState;

class InitState : public GameState
{

public:
    using GameState::GameState;

protected:
    void updateImpl(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter) override;
};