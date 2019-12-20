#include <state/DefaultLController.h>
#include <game/Level.h>
#include <utils/Assert.h>
#include <actions/ActionDefs.h>

#include <components/TilePosition.h>
#include <components/Actor.h>
#include <components/Description.h>

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
            // TBD
            break;
        }
        case RIGHT_MOUSE_BUTTON:
        {
            // Grab the actions possible at the given tile RE. the player
            EntityRef player = m_level->getPlayer()->ref();
            auto tileCoords = m_level->screenCoordsToTile(evt.screenPos);
            auto actions = m_level->actionsForPosition(player, tileCoords);
            UI::ContextMenuList cml;

            // If we've got no actions, do nothing
            if ( !actions.empty() )
            {
                for ( auto const& item : actions )
                {
                    cml.push_back( item->description() );
                }

                // Otherwise, open a context menu containing the actions
                m_level->ui().openContextMenu(cml, evt.screenPos, [actList = std::move(actions), player] (auto arg) {
                    actList[arg]->doAction();
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
    // TODO more robust key handling
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

    // Get the tile positon we would move into, and get the default action for that tile
    // For an empty tile it's to move into it, for a door its to open, for an enemy attack, etc.
    Vector2i newPos = tpos->position + delta;
    auto def = m_level->getDefaultAction(ref, newPos);

    // If there's no action for the tile, do nothing (and don't consume a turn)
    if (def)
    {
        m_level->getComponents<Components::Actor>(ref)->nextAction = def;
        m_level->events().broadcast<GEvents::GameTick>();
    }
}

void DefaultLController::onHoveredTileChange(Vector2i prev, Vector2i curr)
{
    m_level->ui().removeTileHighlight();

	if (!m_level->grid().inBounds(curr))
	{
		return;
	}

    // Only let us interact with things which we can actually see
    if ( m_level->grid().fov().valueAt(curr) != Rules::Visibility::Visible )
    {
        return;
    }

    auto ents = m_level->grid().entitiesAtTile(curr);

    // Highlight hovered entities
    if ( m_level->grid().inBounds( curr ) && !ents.empty() )
    {
        m_level->ui().addTileHighlight( m_level->tileCoordsToScreen(curr) );
    }

    // Grab the first entity under the mouse to have a description
    EntityRef entDesc = EntityNull;
    for ( auto const& e : ents )
    {
        if ( m_level->entityHas<Components::Description>(e) )
        {
            entDesc = e;
            break;
        }
    }

    // Set our description label element to the description of the hovered entity
    auto uiDescLabel = m_level->ui().withId("trf-label")->asType<UI::TextNode>();
    if ( entDesc == EntityNull )
    {
        uiDescLabel->clearText();
    }
    else
    {
        auto descCmp = m_level->getComponents<Components::Description>(entDesc);
        uiDescLabel->setText( descCmp->descriptions[descCmp->current] );
    }

}
