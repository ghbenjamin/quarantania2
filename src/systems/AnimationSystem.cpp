#include <systems/AnimationSystem.h>

#include <game/Level.h>
#include <graphics/RenderInterface.h>
#include <components/PositionComponent.h>
#include <components/RenderSystem.h>
#include <components/AnimationComponent.h>

AnimationSystem::AnimationSystem(Level *parent)
        : System(parent)
{
    m_level->events().subscribe<GameEvents::EntityMove>(this);
}

void AnimationSystem::update(uint32_t ticks, RenderInterface &rInter)
{
    for ( auto &[render, tile, anim] : m_level->ecs().entitiesWith<RenderComponent, PositionComponent, AnimationComponent>() )
    {
        if ( anim->movementPathAnim.has_value() )
        {
            anim->movementPathAnim.reset();
        }
    }
}

void AnimationSystem::operator()( GameEvents::EntityMove &evt )
{
    auto animC = m_level->ecs().getComponents<AnimationComponent>( evt.ent );

    TileAnimationPath pathAnim;

    if ( evt.path.has_value() )
    {
        pathAnim.path = *evt.path;
    }
    else
    {
        pathAnim.path.push_back( evt.oldPos );
        pathAnim.path.push_back( evt.newPos );
    }

    pathAnim.currentPxPos = evt.oldPos.convert<float>();
    pathAnim.nextTile = pathAnim.path[1];

    animC->movementPathAnim = std::move(pathAnim);
}
