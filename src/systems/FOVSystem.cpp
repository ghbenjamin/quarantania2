#include <systems/FOVSystem.h>
#include <game/Level.h>
#include <graphics/Primatives.h>
#include <graphics/RenderInterface.h>
#include <components/PositionComponent.h>
#include <components/ActorComponent.h>
#include <utils/GlobalConfig.h>

FOVSystem::FOVSystem(Level *parent) : System(parent),
   m_fovHidden{ createRectangle({GlobalConfig::TileSizePx, GlobalConfig::TileSizePx}, Colour::Black) },
   m_fovFog{ createRectangle({GlobalConfig::TileSizePx, GlobalConfig::TileSizePx}, Colour::Black.withAlpha(100)) }
{
    m_fovHidden.setRenderLayer(RenderLayer::FOV);
    m_fovFog.setRenderLayer(RenderLayer::FOV);

    m_level->events().subscribe<GameEvents::EntityMove>(this );
    m_level->events().subscribe<GameEvents::LevelReady>(this );
    m_level->events().subscribe<GameEvents::EntityOpenClose>(this );
}

void FOVSystem::operator()(GameEvents::EntityMove& evt)
{
    recalculateFOV();
}

void FOVSystem::update(uint32_t ticks, RenderInterface &rInter)
{
//    Vector2i currPos;
//
//    int tcount =  m_level->grid().bounds().area();
//    for ( int i = 0; i < tcount; i++ )
//    {
//        currPos = m_level->grid().idxToPos(i) * GlobalConfig::TileSizePx;
//        auto visibility = m_level->grid().fov().valueAt(i);
//
//        // If the current tile is hidden, block it out entirely with a black square. If the tile is explored
//        // but not visible, overlay it with a partially transparent black square (fog of war)
//        if ( visibility == Visibility::Hidden )
//        {
//            rInter.addWorldItem( m_fovHidden.renderObject(currPos) );
//        }
//        else if ( visibility == Visibility::Explored )
//        {
//            rInter.addWorldItem( m_fovFog.renderObject(currPos) );
//        }
//    }
}

void FOVSystem::operator()(GameEvents::LevelReady& evt)
{
    // Remove this to turn off 'explored' mode
    m_level->grid().exploreAllTiles();

    recalculateFOV();
}

void FOVSystem::recalculateFOV()
{
    std::vector<FOVObserver> sources;

    for ( auto const&[actor, position] : m_level->ecs().entitiesWith<ActorComponent, PositionComponent>() )
    {
        if (actor->actorType == ActorType::PC)
        {
            sources.push_back({position->tilePosition, 15 /* TODO: Don't hard code this */ });
        }
    }

    m_level->grid().calculateFOV(sources);
}

void FOVSystem::operator()(GameEvents::EntityOpenClose& evt)
{
    recalculateFOV();
}
