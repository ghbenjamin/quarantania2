#include <systems/FOVSystem.h>
#include <game/Level.h>
#include <graphics/Primatives.h>
#include <graphics/RenderInterface.h>
#include <components/PositionComponent.h>
#include <utils/GlobalConfig.h>

FOVSystem::FOVSystem(Level *parent) : System(parent),
   m_fovHidden{ createRectangle({GlobalConfig::TileSizePx, GlobalConfig::TileSizePx}, Colour::Black) },
   m_fovFog{ createRectangle({GlobalConfig::TileSizePx, GlobalConfig::TileSizePx}, Colour::Black.withAlpha(100)) }
{
    m_fovHidden.setRenderLayer(RenderLayer::FOV);
    m_fovFog.setRenderLayer(RenderLayer::FOV);

    m_level->events().subscribe<GEvents::EntityMove>( this );
    m_level->events().subscribe<GEvents::LevelReady>( this );
    m_level->events().subscribe<GEvents::EntityOpenClose>( this );
}

void FOVSystem::accept(GEvents::EntityMove *evt)
{
//    // If the player has moved, recalculate our cached FOV data
//    if ( m_level->isPlayer( evt->ent ) )
//    {
//        m_level->grid()
//               .calculateFOV(evt->newPos, 10 /*TODO DELETE*/ );
//    }
}

void FOVSystem::update(uint32_t ticks, RenderInterface &rInter)
{
    Vector2i currPos;

    int tcount =  m_level->grid().bounds().area();
    for ( int i = 0; i < tcount; i++ )
    {
        currPos = m_level->grid().idxToPos(i) * GlobalConfig::TileSizePx;
        auto visibility = m_level->grid().fov().valueAt(i);

        // If the current tile is hidden, block it out entirely with a black square. If the tile is explored
        // but not visible, overlay it with a partially transparent black square (fog of war)
        if ( visibility == Visibility::Hidden )
        {
            rInter.addWorldItem( m_fovHidden.renderObject(currPos) );
        }
        else if ( visibility == Visibility::Explored )
        {
            rInter.addWorldItem( m_fovFog.renderObject(currPos) );
        }
    }
}

void FOVSystem::accept(GEvents::LevelReady *evt)
{
    // Remove this to turn off 'explored' mode
    m_level->grid().exploreAllTiles();

    recalculateFOV();
}

void FOVSystem::recalculateFOV()
{
    // TODO GetPlayer
//    auto playerRef = m_level->getPlayer()->ref();
//    auto playerPos = m_level->getComponents<PositionComponent>( playerRef )->position;
//    m_level->grid().calculateFOV(playerPos, 10 /*TODO DELETE*/ );
}

void FOVSystem::accept(GEvents::EntityOpenClose *evt)
{
    recalculateFOV();
}
