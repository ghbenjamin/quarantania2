#include <state/DefaultLController.h>
#include <game/Level.h>
#include <utils/Assert.h>

DefaultLController::DefaultLController(Level *level)
: LevelController(level)
{
}

bool DefaultLController::onMouseMove(IEventMouseMove evt)
{
    return false;
}

bool DefaultLController::onMouseDown(IEventMouseDown evt)
{
    static constexpr int LEFT_MOUSE_BUTTON = 1;
    static constexpr int RIGHT_MOUSE_BUTTON = 3;

    switch (evt.button)
    {
        case LEFT_MOUSE_BUTTON:
        {
            break;
        }
        case RIGHT_MOUSE_BUTTON:
        {
            auto tileCoords = m_level->screenCoordsToTile(evt.screenPos);
            auto ents = m_level->grid().entitiesAtTile( tileCoords );
            if ( !ents.empty() )
            {
                // TODO: Work out which entities are here. Work out the possible actions for each.
                // Display these actions here, and respond accordingly to the chosen action.

                UI::ContextMenuList cml = {
                        "Hello", "World", "I", "am", "an", "ENTITY"
                };

                m_level->ui().openContextMenu(cml, evt.screenPos, [](auto arg) {
                    // TODO: Do something other than parrot back the chosen item
                    Logging::log( arg );
                });
            }
            break;
        }
        default:
            break;
    }

    return false;
}

bool DefaultLController::onKeyDown(IEventKeyPress evt)
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


void DefaultLController::doMovePlayer(SDL_Keycode kcode)
{
    auto ref = m_level->getPlayer()->ref();
    auto tpos = m_level->getComponents<Components::TilePosition>(ref);

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

    if ( m_level->grid().pass().valueAt(newPos) != Rules::Passibility::Impassable )
    {
        m_level->events().broadcast<GEvents::EntityMove>( ref, tpos->position, newPos );
    }
}

void DefaultLController::onHoveredTileChange(Vector2i prev, Vector2i curr)
{
    m_level->ui().addTileHighlight(
        Colour::Lavender,
        m_level->tileCoordsToScreen(curr)
    );
}
