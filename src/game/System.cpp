#include <game/System.h>
#include <game/Component.h>
#include <game/Level.h>
#include <graphics/RenderInterface.h>
#include <resource/Sprite.h>

System::System(Level *parent) : m_level(parent) { }
void System::update(uint32_t ticks, RenderInterface &rInter) { }

Systems::Render::Render(Level *parent) : System(parent)
{

}

void Systems::Render::update(uint32_t ticks, RenderInterface &rInter)
{
    for ( auto &[render, tile] : m_level->with<Components::Render, Components::TilePosition>() )
    {
        rInter.addWorldItem(render->sprite.renderObject( tile->position * 16 )); // TODO Hack
    }
}

Systems::Collision::Collision(Level *parent) : System(parent)
{
    m_level->events().subscribe<GEvents::EntityMove>( this );
}

void Systems::Collision::accept(GEvents::EntityMove *evt)
{
}

Systems::Position::Position(Level *parent) : System(parent)
{
   m_level->events().subscribe<GEvents::EntityMove>( this );
}

void Systems::Position::accept(GEvents::EntityMove *evt)
{
    m_level->get<Components::TilePosition>(evt->ent)->position = evt->newPos;
}