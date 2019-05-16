#include <state/LevelState.h>
#include <game/LevelConfig.h>
#include <game/Level.h>
#include <game/LevelFactory.h>
#include <utils/Logging.h>


LevelState::LevelState( LevelConfig const& config )
{
    // Create a blank starting context
    m_pContext = std::make_unique<LevelContext>();

    // Create the initial level
    m_level = LevelFactory::create( config, m_pContext );

    debugStuff();
}


bool LevelState::input(SDL_Event &evt)
{
    return m_level->input(evt);
}

void LevelState::update(uint32_t ticks, RenderInterface &rInterface)
{
    m_ecs.update(ticks, rInterface);
//    m_level->update(ticks, rInterface);
}

void LevelState::debugStuff()
{
    auto ent1 = m_ecs.createEntity();
    m_ecs.addComponent<Components::TilePosition>(ent1, Vector2i{1, 2} );
    m_ecs.addComponent<Components::Render>(ent1, Sprite() );

    auto ent2 = m_ecs.createEntity();
    m_ecs.addComponent<Components::TilePosition>(ent2, Vector2i{3, 4} );

    auto ent3 = m_ecs.createEntity();

    auto ent4 = m_ecs.createEntity();
    m_ecs.addComponent<Components::TilePosition>(ent4, Vector2i{5, 6} );
    m_ecs.addComponent<Components::Render>(ent4, Sprite() );

    auto ent5 = m_ecs.createEntity();
    m_ecs.addComponent<Components::TilePosition>(ent5, Vector2i{7, 8} );
    m_ecs.addComponent<Components::Render>(ent5, Sprite() );
    m_ecs.addComponent<Components::Collider>(ent5);

    for ( auto &[tc] : m_ecs.with<Components::TilePosition>() )
    {
        Logging::log( tc->position );
    }

    Logging::log( "-------------" );

    for ( auto &[tc, rc] : m_ecs.with<Components::TilePosition, Components::Render>() )
    {
        Logging::log( tc->position );
    }


    int a = 5;
}

