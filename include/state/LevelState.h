#pragma once

#include <state/State.h>


class Level;
struct PartyData;
struct LevelContext;

class LevelState : public GameState
{
public:

    LevelState(std::shared_ptr<LevelContext> const& ctx, PartyData const& pgen );
    ~LevelState() override = default;

    bool input(IEvent &evt) override;
    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter) override;

private:

    std::unique_ptr<Level> m_level;
    std::shared_ptr<LevelContext> m_levelCtx;
};