#include <game/System.h>
#include <game/Component.h>
#include <game/Level.h>
#include <graphics/RenderInterface.h>
#include <resource/Sprite.h>
#include <graphics/Primatives.h>
#include <utils/Assert.h>

System::System(Level *parent) : m_level(parent) { }
void System::update(uint32_t ticks, RenderInterface &rInter) { }

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

Systems::Collision::Collision(Level *parent) : System(parent)
{
    m_level->events().subscribe<GEvents::EntityMove>( this );
    m_level->events().subscribe<GEvents::EntityReady>( this );
}

void Systems::Collision::accept(GEvents::EntityMove *evt)
{
    if ( m_level->entityHas<Components::Collider>(evt->ent) )
    {
        m_level->grid().pass().setDynamic( evt->newPos, evt->ent, Rules::Passibility::Impassable );
        m_level->grid().pass().removeDynamic(  evt->oldPos, evt->ent, Rules::Passibility::Impassable );
    }
}

void Systems::Collision::accept(GEvents::EntityReady *evt)
{
    if ( m_level->entityHas<Components::Collider, Components::TilePosition>(evt->ent) )
    {
        auto const& pos = m_level->getComponents<Components::TilePosition>(evt->ent);
        m_level->grid().pass().setDynamic( pos->position, evt->ent, Rules::Passibility::Impassable );
    }
}

Systems::Position::Position(Level *parent) : System(parent)
{
    m_level->events().subscribe<GEvents::EntityMove>( this );
    m_level->events().subscribe<GEvents::EntityReady>( this );
}

void Systems::Position::accept(GEvents::EntityMove *evt)
{
    m_level->getComponents<Components::TilePosition>(evt->ent)->position = evt->newPos;

    m_level->grid().removeEntFromTile( evt->oldPos, evt->ent );
    m_level->grid().addEntToTile( evt->newPos, evt->ent );
}

void Systems::Position::accept(GEvents::EntityReady *evt)
{
    if ( m_level->entityHas<Components::TilePosition>(evt->ent) )
    {
        auto const& pos = m_level->getComponents<Components::TilePosition>(evt->ent);
        m_level->grid().addEntToTile( pos->position, evt->ent );
    }
}

Systems::FOV::FOV(Level *parent) : System(parent),
    m_fovHidden{ createRectangle({16, 16}, Colour::Black) },
    m_fovFog{ createRectangle({16, 16}, Colour::Black.withAlpha(100)) }
{
    m_level->events().subscribe<GEvents::EntityMove>( this );
}

void Systems::FOV::accept(GEvents::EntityMove *evt)
{

}

void Systems::FOV::update(uint32_t ticks, RenderInterface &rInter)
{
    Vector2i curr;
    auto width = m_level->bounds().x();
    for ( int i = 0; i < m_level->tileCount(); i++ )
    {

        auto val = m_level->grid().fov().valueAt( i );
        curr = Vector2i{ i % width, i / width } * 16;

        // Uncomment me once FOV is implemented
//        if ( val == Rules::Visibility::Explored )
//        {
//            rInter.addWorldItem( m_fovFog.renderObject(curr) );
//        }
//        else if ( val == Rules::Visibility::Hidden )
//        {
//            rInter.addWorldItem( m_fovHidden.renderObject(curr) );
//        }
    }
}

Systems::FixedState::FixedState(Level *parent) : System(parent)
{
    m_level->events().subscribe<GEvents::FixedStateChange>( this );
}

void Systems::FixedState::accept(GEvents::FixedStateChange *evt)
{
    auto compFs = m_level->tryGetComponent<Components::FixedState>(evt->ent);
    Assert(!!compFs);
    compFs->current = evt->state;

    auto cmpRender = m_level->tryGetComponent<Components::Render>(evt->ent);
    if ( cmpRender )
    {
        cmpRender->current = evt->state;
    }

    auto cmpDesc = m_level->tryGetComponent<Components::Description>(evt->ent);
    if ( cmpDesc )
    {
        cmpDesc->current = evt->state;
    }
}