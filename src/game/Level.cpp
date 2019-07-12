#include <utility>

#include <game/Level.h>
#include <graphics/RenderInterface.h>
#include <game/InputInterface.h>
#include <game/GEventDefs.h>
#include <utils/Logging.h>
#include <utils/Assert.h>
#include <ui/TextNode.h>
#include <graphics/Primatives.h>

Level::Level(ImmutableLevelData&& imd, LevelContextPtr ctx)
: m_imData(imd), m_ctx(std::move(ctx)), m_passGrid(m_imData.levelSize),
    m_visGrid(m_imData.levelSize), m_lightGrid(m_imData.levelSize)
{
    registerComponent<Components::Render>();
    registerComponent<Components::TilePosition>();
    registerComponent<Components::Collider>();

    registerSystem<Systems::Render>();
    registerSystem<Systems::Position>();
    registerSystem<Systems::Collision>();
    registerSystem<Systems::FOV>();

    m_passGrid.disableCache();
    for ( size_t i = 0; i < m_imData.baseTilemap.size(); i++ )
    {
        switch ( m_imData.baseTilemap[i] )
        {
            case LD::BaseTileType::Entrance:
            case LD::BaseTileType::Wall:
                m_passGrid.setFixed( i, Rules::Passibility::Impassable );
                break;

            case LD::BaseTileType::Exit:
            case LD::BaseTileType::Floor:
            case LD::BaseTileType::Junction:
                m_passGrid.setFixed( i, Rules::Passibility::Passable );
                break;
        }
    }
    m_passGrid.enableCache();
}

bool Level::input(IEvent &evt)
{
    switch ( evt.type )
    {
        case IEventType::KeyPress:
            return handleKeyInput(evt.keyPress);
        case IEventType::MouseClick:
            break;
        case IEventType::MouseMove:
            break;
        case IEventType::WindowResize:
            break;
    }

    return false;
}

bool Level::handleKeyInput(IEventKeyPress &evt)
{
    switch ( evt.keyCode )
    {
        case SDLK_w:
        case SDLK_a:
        case SDLK_s:
        case SDLK_d:
            doMovePlayer( evt.keyCode );
            break;

        default:
            break;
    }

    return false;
}

void Level::render(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter)
{
    renderTiles(ticks, rInter);
    auto ptr = createRectangle( {10, 10}, Colour::Apricot );

}

void Level::update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter)
{
    updateCamera(ticks, iinter, rInter);

    // Render ourself: tiles, etc.
    render(ticks, iinter, rInter);

    // Render everything managed by the ECS
    for ( auto const& sys : m_systems )
    {
        sys->update( ticks, rInter );
    }

    // Render the GUI
    m_uiManager.update(ticks, iinter, rInter);
}

void Level::renderTiles(uint32_t ticks, RenderInterface &rInter)
{
    Vector2i offset;
    Vector2i currPos;
    int row, col;
    int width = m_imData.levelSize.x();

    for ( auto const& layer : m_imData.mapRendering )
    {
        offset = {0, 0};
        row = 0;
        col = 0;

        for ( auto const& ref : layer )
        {
            if ( ref >= 0 )
            {
                currPos = offset + Vector2i{ col * m_imData.tilePixelSize, row * m_imData.tilePixelSize };
                rInter.addWorldItem( m_imData.renderTileMap.get(ref).sprite.renderObject(currPos) );
            }

            col++;
            if ( col >= width )
            {
                col = 0;
                row++;
            }
        }
    }
}

void Level::updateCamera(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    rInter.camera().setBounds( m_imData.levelSize * m_imData.tilePixelSize );

    float scrollSpeed = 0.8f;

    if ( iinter.anyHeld() )
    {
        Vector2f delta = { 0.0, 0.0 };
        if ( iinter.isHeld( SDLK_LEFT ) )
        {
            delta += {-1.0, 0};
        }
        if ( iinter.isHeld( SDLK_RIGHT ) )
        {
            delta += {1.0, 0};
        }
        if ( iinter.isHeld( SDLK_UP ) )
        {
            delta += {0, -1.0};
        }
        if ( iinter.isHeld( SDLK_DOWN ) )
        {
            delta += {0, 1.0};
        }

        if ( delta.x() != 0 || delta.y() != 0 )
        {
            rInter.camera().moveBy(delta * (scrollSpeed * ticks));
        }
    }
}

void Level::doMovePlayer(SDL_Keycode kcode)
{
    auto ref = m_player->ref();
    auto tpos = getComponents<Components::TilePosition>(ref);

    Vector2i delta;

    switch (kcode)
    {
        case SDLK_w:
            delta = {0, -1};
            break;
        case SDLK_a:
            delta = {-1, 0};
            break;
        case SDLK_s:
            delta = {0, 1};
            break;
        case SDLK_d:
            delta = {1, 0};
            break;
        default:
            AssertAlways();
            break;
    }

    Vector2i newPos = tpos->position + delta;

    if ( m_passGrid.valueAt(newPos) != Rules::Passibility::Impassable )
    {
        m_gevents.broadcast<GEvents::EntityMove>( ref, tpos->position, newPos );
    }
}

EntityRef Level::createEntity()
{
    // Requisition a new ID from the pool, but dont construct any new
    // component objects
    return m_entityPool.next();
}

void Level::deleteEntity(EntityRef ent)
{
    // Delete all the components attached to this entity
    for ( auto &[k, v] : m_components )
    {
        auto it = v.find(ent);
        if ( it != v.end() )
        {
            v.erase(it);
        }
    }

    // Put the ID back into the pool
    m_entityPool.release(ent);
}

void Level::disableEntity(EntityRef ent)
{
    for ( auto &[k, v] : m_components )
    {
        auto it = v.find(ent);
        if ( it != v.end() )
        {
            // it->second.DISABLE_ME
        }
    }
}

void Level::enableEntity(EntityRef ent)
{
    for ( auto &[k, v] : m_components )
    {
        auto it = v.find(ent);
        if ( it != v.end() )
        {
            // it->second.ENABLE_ME
        }
    }
}

GEventHub &Level::events()
{
    return m_gevents;
}

void Level::setPlayer(std::unique_ptr<Player> &&player)
{
    m_player = std::move(player);
}

GridFeature<Rules::Passibility, EntityRef>& Level::passGrid()
{
    return m_passGrid;
}

GridFeature<Rules::Visibility, EntityRef>& Level::fovGrid()
{
    return m_visGrid;
}

GridFeature<Rules::LightLevel, EntityRef>& Level::lightGrid()
{
    return m_lightGrid;
}

ImmutableLevelData const &Level::data()
{
    return m_imData;
}


