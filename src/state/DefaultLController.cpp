#include <state/DefaultLController.h>
#include <game/Level.h>
#include <utils/Assert.h>
#include <game/ActionDefs.h>

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
//            // Grab the actions possible at the given tile RE. the player
//            EntityRef player = m_level->getPlayer()->ref();
//            auto tileCoords = m_level->screenCoordsToTile(evt.screenPos);
//            auto actions = m_level->actionsForPosition(player, tileCoords);
//            UI::ContextMenuList cml;
//
//            // If we've got no actions, do nothing
//            if ( !actions.empty() )
//            {
//                for ( auto const& item : actions )
//                {
//                    cml.push_back( item->description() );
//                }
//
//                // Otherwise, open a context menu containing the actions
//                m_level->ui().openContextMenu(cml, evt.screenPos, [actList = std::move(actions), player] (auto arg) {
//                    actList[arg]->doAction();
//                });
//            }

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
        default:
            break;
    }

    return false;
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
    if ( m_level->grid().fov().valueAt(curr) != Visibility::Visible )
    {
        return;
    }

    auto ents = m_level->grid().entitiesAtTile(curr);


    if ( !ents.empty() )
    {
        // Highlight hovered entities
        m_level->ui().addTileHighlight( m_level->tileCoordsToScreen(curr) );

        std::vector<UI::TooltipData> tooltipData;

        for ( auto const& ent : ents )
        {
            if ( m_level->entityHas<DescriptionComponent>(ent) )
            {
                auto descC = m_level->getComponents<DescriptionComponent>( ent );
                tooltipData.emplace_back( descC->title, descC->shortDescription, descC->longDescription );
            }
        }

        if ( !tooltipData.empty() )
        {
            m_level->ui().openTooltip( tooltipData, m_level->tileCoordsToScreen(curr) + Vector2i{30, 30}  );
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
