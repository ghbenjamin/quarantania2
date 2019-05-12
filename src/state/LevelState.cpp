#include <state/LevelState.h>
#include <game/LevelConfig.h>
#include <game/Level.h>
#include <game/LevelFactory.h>


LevelState::LevelState( LevelConfig const& config )
{
    // Create a blank starting context
    m_pContext = std::make_unique<LevelContext>();

    // Create the initial level
    m_level = LevelFactory::create( config, m_pContext );
}


bool LevelState::input(SDL_Event &evt)
{
    return m_level->input(evt);
}

void LevelState::update(Uint32 ticks, RenderInterface &rInterface)
{
    m_level->update(ticks, rInterface);
}

