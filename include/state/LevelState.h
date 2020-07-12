#pragma once

#include <state/State.h>
#include <game/Level.h>


struct LevelConfig;
struct PCData;

class LevelState : public GameState
{
public:

    LevelState(LevelConfig const& config, LevelContextPtr ctx, PartyData const& pgen );
    ~LevelState() override = default;

    bool input(IEvent &evt) override;
    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter) override;

private:

    LevelPtr m_level;
    LevelContextPtr m_levelCtx;
};