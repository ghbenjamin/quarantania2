#include <systems/FOVSystem.h>
#include <game/Level.h>
#include <graphics/Primatives.h>
#include <graphics/RenderInterface.h>
#include <components/PositionComponent.h>
#include <components/ActorComponent.h>
#include <utils/GlobalConfig.h>
#include <components/LightingComponent.h>

FOVSystem::FOVSystem(Level *parent) : System(parent),
   m_fovHidden{ createRectangle(GlobalConfig::TileDimsPx, Colour::Black) },
   m_fovFog{ createRectangle(GlobalConfig::TileDimsPx, Colour::Black.withAlpha(100)) },
   m_renderObj{0, ShaderType::ColourShader},
   m_renderDirtyBit(true)
{
    m_level->events().subscribe<GameEvents::EntityMove>(this );
    m_level->events().subscribe<GameEvents::LevelReady>(this );
}

void FOVSystem::operator()(GameEvents::EntityMove& evt)
{
    recalculateFOV();
}

void FOVSystem::update(uint32_t ticks, RenderInterface &rInter)
{
    if (m_renderDirtyBit)
    {
        rInter.releaseRenderQueue(RenderLayer::FOV);
        rInter.addItem( m_renderObj, RenderLayer::FOV );
        rInter.holdRenderQueue(RenderLayer::FOV);
        m_renderDirtyBit = false;
    }
}

void FOVSystem::operator()(GameEvents::LevelReady& evt)
{
    // Remove this to turn off 'explored' mode
    m_level->grid().exploreAllTiles();

    recalculateFOV();
}

void FOVSystem::recalculateFOV()
{
    // Work out which actors are acting as FOV sources
    std::vector<FOVObserver> observers;
    for ( auto const&[actorC, posC] : m_level->ecs().entitiesWith<ActorComponent, PositionComponent>() )
    {
        if (actorC->actorType == ActorType::PC)
        {
            observers.push_back({ posC->tilePosition, 15 /* TODO: Don't hard code this */ });
        }
    }
    
    // Set the corrent FOV level for our level grid using the above sources
    m_level->grid().calculateFOV(observers);
    
    // Work out which entities are acting as light sources
    std::vector<LightSource> lightSources;
    for ( auto const&[lightC, posC] : m_level->ecs().entitiesWith<LightingComponent, PositionComponent>() )
    {
        lightSources.push_back({ posC->tilePosition, lightC->intensity, lightC->colour });
    }

    // Set the current light levels using the above light sources
    m_level->grid().calculateLightLevel(lightSources);

    // Reset our render object
    m_renderObj = {0, ShaderType::ColourShader};

    // Update our render object with the correct colour for each tile
    Vector2i currPos;
    int tcount =  m_level->grid().bounds().area();
    
    for ( int i = 0; i < tcount; i++ )
    {
        currPos = m_level->tileIdxToWorld(i);
        auto visibility = m_level->grid().fov().valueAt(i);
        auto lighting = m_level->grid().light().valueAt(i);

        // If the current tile is hidden, block it out entirely with a black square. If the tile is explored
        // but not visible, overlay it with a partially transparent black square (fog of war)
        if ( visibility == Visibility::Hidden )
        {
            m_renderObj.merge( m_fovHidden.renderObject(currPos) );
        }
        else if ( visibility == Visibility::Explored )
        {
            m_renderObj.merge( m_fovFog.renderObject(currPos) );
        }
        
        if (lighting > 0.01)
        {
            auto yellowObjs = m_fovFog.renderObject(currPos);
            yellowObjs.setColourVerts( 0, 1.000f, 0.980f, 0.804f, lighting * 0.5f );
            m_renderObj.merge(yellowObjs);
        }
    }

    // Mark that we need to push the new render overlay data to the renderer
    m_renderDirtyBit = true;
}
