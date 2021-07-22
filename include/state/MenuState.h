#pragma once

#include <state/GameState.h>

class MenuState : public GameState
{

public:
    using GameState::GameState;
    
protected:
    bool inputImpl(IEvent &evt) override;
    void updateImpl(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter) override;

};