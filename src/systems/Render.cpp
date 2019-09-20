#include <systems/Render.h>
#include <game/Level.h>
#include <graphics/RenderInterface.h>

Systems::Render::Render(Level *parent) : System(parent)
{
}

void Systems::Render::update(uint32_t ticks, RenderInterface &rInter)
{
    for ( auto &[render, tile] : m_level->entitiesWith<Components::Render, Components::TilePosition>() )
    {
        rInter.addWorldItem(render->sprites[render->current].renderObject( tile->position * 16 )); // TODO Hack
    }
}