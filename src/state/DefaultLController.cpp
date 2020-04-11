#include <state/DefaultLController.h>
#include <game/Level.h>
#include <utils/Assert.h>
#include <actions/ActionDefs.h>

#include <components/PositionComponent.h>
#include <components/ActorComponent.h>
#include <components/DescriptionComponent.h>
#include <utils/GlobalConfig.h>
#include <components/ItemComponent.h>

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
    auto tpos = m_level->getComponents<PositionComponent>(ref);

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

    tryDefaultAction(tpos->position + delta);
}

void DefaultLController::onHoveredTileChange(Vector2i prev, Vector2i curr)
{
    m_level->ui().removeTileHighlight();
    m_level->ui().closeTooltip();

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
    if ( !ents.empty() )
    {
        m_level->ui().addTileHighlight( m_level->tileCoordsToScreen(curr) );
    }

    if ( ents.size() == 1 )
    {
        if ( m_level->entityHas<ItemComponent>(ents[0]) )
        {
            auto itemComp = m_level->getComponents<ItemComponent>( ents[0] );
            m_level->ui().openTooltip( itemComp->item->tooltipData(), m_level->tileCoordsToScreen(curr) + Vector2i{20, 20}  );
        }
    }
}

void DefaultLController::update(std::uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
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
        }
    }
}

void DefaultLController::tryDefaultAction(Vector2i playerLoc)
{
    auto ref = m_level->getPlayer()->ref();
    auto defAction = m_level->getDefaultAction(ref, playerLoc);

    if (defAction)
    {
        m_level->getComponents<ActorComponent>(ref)->nextAction = defAction;
        m_level->events().broadcast<GEvents::GameTick>();

        // This action may have moved the player - recentre the camera
        m_level->camera().centreOnTile(playerLoc);
    }
}