#include <controllers/LevelController.h>
#include <game/Level.h>
#include <game/ActionDefs.h>
#include <game/GameEventDefs.h>
#include <components/ItemComponent.h>
#include <components/ActorComponent.h>
#include <components/PositionComponent.h>
#include <ui/TileHighlights.h>

LevelController::LevelController(Level *level)
: m_level(level) { }

bool LevelController::input(IEvent &evt)
{
    switch ( evt.type )
    {
        case IEventType::KeyPress:
        {
            onKeyDown(evt.keyPress);
        }

        case IEventType::MouseDown:
        {
            onMouseDown(evt.mouseDown);
        }

        case IEventType::MouseUp:
        {
            onMouseUp(evt.mouseUp);
        }

        case IEventType::MouseMove:
        {
             onMouseMove(evt.mouseMove);

             auto currTile = m_level->screenCoordsToTile(evt.mouseMove.screenPos);
             if ( currTile != m_lastHoveredTile )
             {
                onHoveredTileChange(m_lastHoveredTile, currTile);
                m_lastHoveredTile = currTile;
             }
        }

        default:
            break;
    }

    return true;
}

void LevelController::onHoveredTileChange(Vector2i prev, Vector2i curr)
{
}

bool LevelController::scrollLevel(std::uint32_t ticks, InputInterface &iinter)
{
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
            m_level->camera().moveBy(delta * (float)ticks);
            return true;
        }
    }

    return false;
}



// Default Level Controller
// --------------------------------------


DefaultLController::DefaultLController(Level *level)
        : LevelController(level) { }

bool DefaultLController::onMouseMove(IEventMouseMove evt)
{
    return false;
}

bool DefaultLController::onMouseDown(IEventMouseDown evt)
{
    auto tile = m_level->screenCoordsToTile(evt.screenPos);
    auto ents = m_level->grid().entitiesAtTile(tile);

    // Is there something where we clicked?
    if (!ents.empty())
    {
        for ( EntityRef ref : ents )
        {
            // Is the thing we clicked an actor?
            if ( m_level->ecs().entityHas<ActorComponent>(ref) )
            {
                // Is the thing we clicked a player character?
                auto actorComp = m_level->ecs().getComponents<ActorComponent>(ref);
                if ( actorComp->actorType == ActorType::PC )
                {
                    // Is it the PC whose turn it is?
                    if (ref == m_level->getActiveEntity())
                    {
                        setNextController( std::make_shared<EntityMoveController>(m_level, ref) );
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool DefaultLController::onKeyDown(IEventKeyPress evt)
{
    return false;
}

void DefaultLController::onHoveredTileChange(Vector2i prev, Vector2i curr)
{
    m_level->ui().deleteElement( m_tileHighlight );

    if (!m_level->grid().inBounds(curr))
    {
        return;
    }

    // Only let us interact with things which we can actually see
    if ( m_level->grid().fov().valueAt(curr) != Visibility::Visible )
    {
        return;
    }

    auto ents = m_level->grid().entitiesAtTile(curr);

    if ( !ents.empty() )
    {
        // Highlight hovered entities
        m_tileHighlight = m_level->ui().createElement<UI::SingleTileHighlight>( nullptr, curr );
    }
}

void DefaultLController::update(std::uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    scrollLevel(ticks, iinter);
}

void DefaultLController::onExitSelf()
{
    m_level->ui().deleteElement( m_tileHighlight );
    m_lastHoveredTile = {-1, -1};
}




// Entity Move Controller
// --------------------------------------



EntityMoveController::EntityMoveController(Level* level, EntityRef entity)
        : LevelController(level), m_entity(entity)
{
    auto position = m_level->ecs().getComponents<PositionComponent>(entity);
    m_origin = position->tilePosition;
    m_pathMap = m_level->grid().allPathsFromTile(m_origin, 8);

    GridRegion gr;
    for (auto const&[k, v] : m_pathMap )
    {
        gr.push_back(k);
    }

    m_tileHighlight = m_level->ui().createElement<UI::TileRegionHighlight>(nullptr, gr, Colour::Lime);
}

void EntityMoveController::update(std::uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    scrollLevel(ticks, iinter);
}

bool EntityMoveController::onKeyDown(IEventKeyPress evt)
{
    switch (evt.keyCode)
    {
        case SDLK_ESCAPE:
            popController();
            return true;
            break;
        default:
            break;
    }
    return false;
}

bool EntityMoveController::onMouseDown(IEventMouseDown evt)
{
    auto tile = m_level->screenCoordsToTile(evt.screenPos);

    if ( m_pathMap.find(tile) != m_pathMap.end() )
    {
        m_level->events().broadcast<GameEvents::EntityMove>(m_entity, m_origin, tile, m_tilePath);
        m_level->events().broadcast<GameEvents::EntityAction>(m_entity, 5 /* TODO Actual*/);

        popController();
        return true;
    }


    return false;
}

void EntityMoveController::onHoveredTileChange(Vector2i prev, Vector2i curr)
{
    if ( m_pathMap.find(curr) != m_pathMap.end() )
    {
        m_tilePath = m_level->grid().pathFromPathMap(m_pathMap, curr);
        m_level->ui().deleteElement(m_pathHighlight);
        m_pathHighlight = m_level->ui().createElement<UI::TileRegionHighlight>(nullptr, m_tilePath, Colour::Red);
    }
}

void EntityMoveController::onExitSelf()
{
    m_level->ui().deleteElement(m_tileHighlight);
    m_level->ui().deleteElement(m_pathHighlight);
}


