#include <state/LevelState.h>
#include <game/LevelConfig.h>
#include <game/Level.h>
#include <game/RandomLevelFactory.h>
#include <utils/Logging.h>


LevelState::LevelState(RandomLevelConfig const& config, LevelContextPtr ctx, PartyData const& pdata )
: m_levelCtx(ctx)
{
    // Create the level
    RandomLevelFactory factory;
    m_level = factory.create( config, m_levelCtx, pdata );
}

bool LevelState::input(IEvent &evt)
{
    return m_level->input(evt);
}

void LevelState::update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInterface)
{
    m_level->update(ticks, iinter, rInterface);

    if ( m_level->isComplete() )
    {
        requestPopState();
    }
}

