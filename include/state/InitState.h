#pragma once

#include <state/State.h>
#include <game/LevelContext.h>
#include <game/LevelConfig.h>


class InitState : public GameState
{

public:
    InitState();
    ~InitState() override = default;

    bool input(IEvent &evt) override;
    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter) override;

private:
    RandomLevelConfig generateNextLevelConfig();

    LevelContextPtr m_levelCtx;
};