#include <systems/RenderSystem.h>
#include <game/Level.h>
#include <graphics/RenderInterface.h>

#include <components/PositionComponent.h>
#include <components/RenderComponent.h>

RenderSystem::RenderSystem(Level *parent)
    : System(parent) {}

void RenderSystem::update(uint32_t ticks, RenderInterface &rInter)
{
    for ( auto &[render, tile] : m_level->ecs().entitiesWith<RenderComponent, PositionComponent>() )
    {
        rInter.addWorldItem(render->sprites[render->current].renderObject(tile->pixelPosition));
    }
}