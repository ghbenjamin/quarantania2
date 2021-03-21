#include <systems/FOVSystem.h>
#include <game/Level.h>
#include <graphics/Primatives.h>
#include <graphics/RenderInterface.h>
#include <components/PositionComponent.h>
#include <components/ActorComponent.h>
#include <utils/GlobalConfig.h>

FOVSystem::FOVSystem(Level *parent) : System(parent),
   m_fovHidden{ createRectangle({GlobalConfig::TileSizePx, GlobalConfig::TileSizePx}, Colour::Black) },
   m_fovFog{ createRectangle({GlobalConfig::TileSizePx, GlobalConfig::TileSizePx}, Colour::Black.withAlpha(100)) },
   m_hiddenColour( Colour::Black ),
   m_fovColour( Colour::Black.withAlpha(100) ),
   m_renderObj{0, ShaderType::ColourShader},
   m_renderDirtyBit(true)
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

    std::vector<FOVObserver> sources;

    for ( auto const&[actor, position] : m_level->ecs().entitiesWith<ActorComponent, PositionComponent>() )
    {
        if (actor->actorType == ActorType::PC)
        {
            sources.push_back({position->tilePosition, 15 /* TODO: Don't hard code this */ });
        }
    }

    // Set the corrent FOV level for our level grid using the above sources
    m_level->grid().calculateFOV(sources);

    // Update our FOV render object


    // Reset our render object
    m_renderObj = {0, ShaderType::ColourShader};

    // Update our render object with the correct colour for each tile
    Vector2i currPos;
    Vector2i tileSize = {GlobalConfig::TileSizePx, GlobalConfig::TileSizePx};
    Sprite hiddenSprite = createRectangle(tileSize, m_hiddenColour);
    Sprite fovSprite = createRectangle(tileSize, m_fovColour);
    int tcount =  m_level->grid().bounds().area();
    for ( int i = 0; i < tcount; i++ )
    {
        currPos = m_level->grid().idxToPos(i) * GlobalConfig::TileSizePx;
        auto visibility = m_level->grid().fov().valueAt(i);

        // If the current tile is hidden, block it out entirely with a black square. If the tile is explored
        // but not visible, overlay it with a partially transparent black square (fog of war)
        if ( visibility == Visibility::Hidden )
        {
            m_renderObj.merge( hiddenSprite.renderObject(currPos) );
        }
        else if ( visibility == Visibility::Explored )
        {
            m_renderObj.merge( fovSprite.renderObject(currPos) );
        }
    }

    // Mark that we need to push the new render overlay data to the renderer
    m_renderDirtyBit = true;
}

void FOVSystem::operator()(GameEvents::EntityOpenClose& evt)
{
    recalculateFOV();
}
