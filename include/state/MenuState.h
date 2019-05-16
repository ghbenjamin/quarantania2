#pragma once

#include <state/State.h>

class MenuState : public GameState
{

public:
    bool input(SDL_Event &evt) override;
    void update(uint32_t ticks, RenderInterface &rInter) override;

};