#pragma once

#include <state/State.h>

class Overworld;

class OverworldState : public GameState
{
public:
    
    OverworldState();
    ~OverworldState() override = default;

    bool input(IEvent &evt) override;
    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter) override;

private:
    std::unique_ptr<Overworld> m_overworld;

};