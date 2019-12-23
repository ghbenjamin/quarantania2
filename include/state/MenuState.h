#pragma once

#include <state/State.h>

class MenuState : public GameState
{

public:
    using GameState::GameState;

    bool input(IEvent &evt) override;
    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter) override;

};