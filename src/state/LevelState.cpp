#include <state/LevelState.h>
#include <game/LevelConfig.h>
#include <game/Level.h>
#include <game/LevelFactory.h>
#include <utils/Logging.h>


LevelState::LevelState( LevelConfig const& config, LevelContextPtr ctx )
: m_levelCtx(ctx)
{
    PlayerData pdata;
    pdata.name = "Angus Mackenzie";

    pdata.startingHeldItems.push_back( "Longsword" );
    pdata.startingHeldItems.push_back( "Rope of Climbing" );
    pdata.startingEquippedItems.push_back( "Amulet of Mighty Fists" );

    // Create the level
    LevelFactory factory;
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

