#pragma once

#include <state/State.h>
#include <game/Level.h>


struct LevelConfig;

class LevelState : public GameState
{
public:

    LevelState( LevelConfig const& config );
    ~LevelState() override = default;

    bool input(SDL_Event &evt) override;
    void update(uint32_t ticks, RenderInterface &rInter) override;

private:

    LevelPtr m_level;
    LevelContextPtr m_pContext;
};