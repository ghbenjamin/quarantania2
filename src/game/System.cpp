#include <game/System.h>
#include <game/Component.h>
#include <game/Level.h>
#include <graphics/RenderInterface.h>
#include <resource/Sprite.h>

System::System(Level *parent) : m_level(parent) { }
void System::update(uint32_t ticks, RenderInterface &rInter) { }
void System::acceptGEvent(GEvent &event) { }


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

void Systems::Render::acceptGEvent(GEvent &event)
{
    switch ( event.type )
    {
        default:
            AssertAlways();
            break;
    }
}

Systems::Collision::Collision(Level *parent) : System(parent)
{
    m_level->events().subscribe( GEventType::EntityMove, this );
}

void Systems::Collision::acceptGEvent(GEvent &event)
{
    switch ( event.type )
    {
        case GEventType::EntityMove:
            break;
        default:
            AssertAlways();
            break;
    }
}

Systems::Position::Position(Level *parent) : System(parent)
{
    m_level->events().subscribe( GEventType::EntityMove, this );
}

void Systems::Position::acceptGEvent(GEvent &event)
{
    switch ( event.type )
    {
        case GEventType::EntityMove:
            m_level->get<Components::TilePosition>(event.entityMove.ent)->position = event.entityMove.newPosition;
            break;
        default:
            AssertAlways();
            break;
    }
}
