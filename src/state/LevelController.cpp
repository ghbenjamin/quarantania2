#include <state/LevelController.h>
#include <game/Level.h>

LevelController::LevelController(Level *level)
: m_level(level)
{

}

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

void LevelController::update(std::uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{

}

bool LevelController::onMouseMove(IEventMouseMove evt)
{
    return false;
}

bool LevelController::onMouseDown(IEventMouseDown evt)
{
    return false;
}

bool LevelController::onMouseUp(IEventMouseUp evt)
{
    return false;
}

bool LevelController::onKeyDown(IEventKeyPress evt)
{
    return false;
}

void LevelController::onHoveredTileChange(Vector2i prev, Vector2i curr)
{

}
