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
    for ( auto &[render, tile] : m_level->entitiesWith<Components::Render, Components::TilePosition>() )
    {
        rInter.addWorldItem(render->sprite.renderObject( tile->position * 16 )); // TODO Hack
    }
}

Systems::Collision::Collision(Level *parent) : System(parent)
{
    m_level->events().subscribe<GEvents::EntityMove>( this );
    m_level->events().subscribe<GEvents::EntityReady>( this );
}

void Systems::Collision::accept(GEvents::EntityMove *evt)
{
    if ( m_level->entityHas<Components::Collider>(evt->ent) )
    {
        m_level->passGrid().setDynamic( evt->newPos, evt->ent, Rules::Passibility::Impassable );
        m_level->passGrid().removeDynamic(  evt->oldPos, evt->ent, Rules::Passibility::Impassable );
    }
}

void Systems::Collision::accept(GEvents::EntityReady *evt)
{
    if ( m_level->entityHas<Components::Collider, Components::TilePosition>(evt->ent) )
    {
        auto const& pos = m_level->getComponents<Components::TilePosition>(evt->ent);
        m_level->passGrid().setDynamic( pos->position, evt->ent, Rules::Passibility::Impassable );
    }
}

Systems::Position::Position(Level *parent) : System(parent)
{
   m_level->events().subscribe<GEvents::EntityMove>( this );
}

void Systems::Position::accept(GEvents::EntityMove *evt)
{
    m_level->getComponents<Components::TilePosition>(evt->ent)->position = evt->newPos;
}