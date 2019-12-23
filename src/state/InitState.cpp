#include <state/InitState.h>
#include <state/LevelState.h>

InitState::InitState()
: m_levelCtx( std::make_shared<LevelContext>() )
{
}

bool InitState::input(IEvent &evt)
{
    return false;
}

void InitState::update(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    auto config = generateNextLevelConfig();
    setNextState<LevelState>( config, m_levelCtx );
}

LevelConfig InitState::generateNextLevelConfig()
{
    LevelConfig config;
    config.size = {41, 25};
    config.roomDensity = 400;

    return config;
}

