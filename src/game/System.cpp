#include <game/System.h>
#include <game/Component.h>
#include <game/ECS.h>
#include <graphics/RenderInterface.h>
#include <resource/Sprite.h>

System::System(ECS *parent) : m_ecs(parent) { }
void System::update(uint32_t ticks, RenderInterface &rInter) { }
void System::acceptGEvent(GEvent &event) { }


Systems::Render::Render(ECS *parent) : System(parent)
{

}

void Systems::Render::update(uint32_t ticks, RenderInterface &rInter)
{
    for ( auto &[render, tile] : m_ecs->with<Components::Render, Components::TilePosition>() )
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

Systems::Collision::Collision(ECS *parent) : System(parent)
{
    m_ecs->events().subscribe( GEventType::EntityMove, this );
}

void Systems::Collision::acceptGEvent(GEvent &event)
{
    switch ( event.type )
    {
        case GEventType::EntityMove:
            fmt::print( "Collision: {} \n", event.entityMove.newPosition.to_string() );
            break;
        default:
            AssertAlways();
            break;
    }
}

Systems::Position::Position(ECS *parent) : System(parent)
{
    m_ecs->events().subscribe( GEventType::EntityMove, this );
}

void Systems::Position::acceptGEvent(GEvent &event)
{
    switch ( event.type )
    {
        case GEventType::EntityMove:
            m_ecs->get<Components::TilePosition>(event.entityMove.ent)->position = event.entityMove.newPosition;
            break;
        default:
            AssertAlways();
            break;
    }
}
