#include <systems/AnimationSystem.h>

#include <game/Level.h>
#include <graphics/RenderInterface.h>
#include <components/RenderComponent.h>
#include <components/AnimationComponent.h>
#include <utils/Memory.h>

AnimationSystem::AnimationSystem(Level *parent)
        : System(parent)
{
    m_level->events().subscribe<GameEvents::EntityMove>(this);
    m_level->events().subscribe<GameEvents::EntityDamage>(this);
}


void AnimationSystem::operator()( GameEvents::EntityMove &evt )
{
    auto animC = m_level->ecs().getComponents<AnimationComponent>( evt.ent );
    
    std::vector<Vector2i> path;

    if ( evt.path.has_value() )
    {
        path = *evt.path;
    }
    else
    {
        path.push_back( evt.oldPos );
        path.push_back( evt.newPos );
    }

    std::vector<Vector2f> worldPath;
    worldPath.reserve( path.size() + 1 );
    worldPath.push_back( m_level->tileCoordsToWorld( evt.oldPos ).convert<float>() );

    for ( Vector2i tile : path )
    {
        worldPath.push_back( m_level->tileCoordsToWorld( tile ).convert<float>() );
    }

    m_level->pushAnimation( Utils::make_unique_with_type<Animation, AnimTilePath>(m_level, evt.ent, worldPath, 0.2f) );
}

void AnimationSystem::operator()( GameEvents::EntityDamage &evt )
{
    if ( m_level->ecs().entityHas<AnimationComponent>( evt.target ) )
    {
        auto [renderC, animC] = m_level->ecs().getComponents<RenderComponent, AnimationComponent>(evt.target);

        m_level->pushAnimation( Utils::make_unique_with_type<Animation, AnimColourMod>(m_level, evt.target,
                Colour::Red, renderC->sprite.getColour(), 0.5f) );
    }
}
