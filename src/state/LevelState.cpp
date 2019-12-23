#include <state/LevelState.h>
#include <game/LevelConfig.h>
#include <game/Level.h>
#include <game/LevelFactory.h>
#include <utils/Logging.h>


LevelState::LevelState( LevelConfig const& config, LevelContextPtr ctx )
: m_levelCtx(ctx)
{
    // Create the level
    LevelFactory factory;
    m_level = factory.create( config, m_levelCtx );
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
        Logging::log("DONE!");
    }
}

