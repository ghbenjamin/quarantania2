#include <systems/FOV.h>
#include <game/Level.h>
#include <graphics/Primatives.h>
#include <graphics/RenderInterface.h>
#include <components/TilePosition.h>

Systems::FOV::FOV(Level *parent) : System(parent),
    m_fovHidden{ createRectangle({16, 16}, Colour::Black) },
    m_fovFog{ createRectangle({16, 16}, Colour::Black.withAlpha(100)) }
{
    m_fovHidden.setRenderLayer(RenderLayer::FOV);
    m_fovFog.setRenderLayer(RenderLayer::FOV);

    m_level->events().subscribe<GEvents::EntityMove>( this );
    m_level->events().subscribe<GEvents::LevelReady>( this );
    m_level->events().subscribe<GEvents::EntityOpenClose>( this );
}

void Systems::FOV::accept(GEvents::EntityMove *evt)
{
    // If the player has moved, recalculate our cached FOV data
    if ( m_level->isPlayer( evt->ent ) )
    {
        m_level->grid()
               .calculateFOV(evt->newPos, 10 /*TODO DELETE*/ );
    }
}

void Systems::FOV::update(uint32_t ticks, RenderInterface &rInter)
{
    Vector2i currPos;

    int tcount =  m_level->tileCount();
    for ( int i = 0; i < tcount; i++ )
    {
        currPos = m_level->grid().idxToPos(i) * 16;
        auto visibility = m_level->grid().fov().valueAt(i);

        // If the current tile is hidden, block it out entirely with a black square. If the tile is explored
        // but not visible, overlay it with a partially transparent black square (fog of war)
        if ( visibility == Rules::Visibility::Hidden )
        {
            rInter.addWorldItem( m_fovHidden.renderObject(currPos) );
        }
        else if ( visibility == Rules::Visibility::Explored )
        {
            rInter.addWorldItem( m_fovFog.renderObject(currPos) );
        }
    }
}

void Systems::FOV::accept(GEvents::LevelReady *evt)
{
    recalculateFOV();
}

void Systems::FOV::recalculateFOV()
{
    auto playerRef = m_level->getPlayer()->ref();
    auto playerPos = m_level->getComponents<Components::TilePosition>( playerRef )->position;
    m_level->grid()
           .calculateFOV(playerPos, 10 /*TODO DELETE*/ );
}

void Systems::FOV::accept(GEvents::EntityOpenClose *evt)
{
    recalculateFOV();
}
