#include <systems/AnimationSystem.h>

#include <game/Level.h>
#include <graphics/RenderInterface.h>
#include <components/PositionComponent.h>
#include <components/RenderComponent.h>
#include <components/AnimationComponent.h>

AnimationSystem::AnimationSystem(Level *parent)
        : System(parent)
{
    m_level->events().subscribe<GameEvents::EntityMove>(this);
    m_level->events().subscribe<GameEvents::EntityDamage>(this);
}

void AnimationSystem::update(uint32_t ticks, RenderInterface &rInter)
{
    for ( auto &[render, tile, anim] : m_level->ecs().entitiesWith<RenderComponent, PositionComponent, AnimationComponent>() )
    {
        if ( anim->movementPathAnim.has_value() )
        {
            anim->movementPathAnim->advance( ticks );
            tile->pixelPosition = anim->movementPathAnim->currentPosition().convert<int>();
            
            if ( anim->movementPathAnim->isComplete() )
            {
                tile->pixelPosition = anim->movementPathAnim->finalPosition().convert<int>();
                anim->movementPathAnim.reset();
            }
        }
        if ( anim->colourModAnim.has_value() )
        {
            anim->colourModAnim->advance( ticks );
            
            if ( anim->colourModAnim->isComplete() )
            {
                render->sprite.setColourMod(anim->colourModAnim->colour());
                anim->colourModAnim.reset();
            }
        }
    }
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
    
    TileAnimationPath pathAnim {worldPath, 0.2f};
    animC->movementPathAnim = std::move(pathAnim);
}

void AnimationSystem::operator()( GameEvents::EntityDamage &evt )
{
    if ( m_level->ecs().entityHas<AnimationComponent>( evt.target ) )
    {
        auto [renderC, animC] = m_level->ecs().getComponents<RenderComponent, AnimationComponent>(evt.target);
        
        animC->colourModAnim = { renderC->sprite.getColour(), 0.5f };
        renderC->sprite.setColourMod(Colour::Red);
    }
}
