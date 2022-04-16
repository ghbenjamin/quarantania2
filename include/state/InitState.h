#pragma once

#include <state/GameState.h>

class RunState;

class InitState : public GameState
{

public:
    InitState( LuaState& luaState );

protected:
    void updateImpl(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter) override;
};