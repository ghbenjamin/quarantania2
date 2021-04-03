#include <state/LevelState.h>
#include <game/LevelConfig.h>
#include <game/Level.h>
#include <utils/Logging.h>
#include <resource/Tiled.h>
#include <game/FixedLevelFactory.h>


LevelState::LevelState(RandomLevelConfig const& config, LevelContextPtr ctx, PartyData const& pdata )
: m_levelCtx(ctx)
{
    TiledMapLoader loader;
    TiledMap tm = loader.load( "../resource/maps/arena.json" );

    FixedLevelFactory ffactory;
    m_level = ffactory.create(&tm, m_levelCtx, pdata);
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

