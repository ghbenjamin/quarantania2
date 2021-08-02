#include <game/LevelController.h>

#include <game/Level.h>
#include <components/ItemComponent.h>
#include <components/ActorComponent.h>
#include <components/PositionComponent.h>
#include <ui/level/ActionPopupMenu.h>
#include <ui/lib/Manager.h>
#include <game/ActionDefs.h>
#include <resource/ResourceManager.h>
#include <ui/level/LevelMainMenu.h>

LevelController::LevelController(Level *level, UI::Manager* ui)
: m_level(level), m_ui(ui), m_shouldPopController(false)
{
    addKeybinding( SDLK_ESCAPE, [this]() {
        this->popController();
    });
}

bool LevelController::inputImpl(IEvent &evt)
{
    switch ( evt.type )
    {
        case IEventType::KeyPress:
        {
            auto it = m_keybinds.find(evt.keyPress.keyCode);
            if (it != m_keybinds.end())
            {
                it->second();
                return true;
            }
            else
            {
                onKeyDown(evt.keyPress);
            }
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

bool LevelController::input(IEvent &evt)
{
    switch ( evt.type )
    {
        case IEventType::KeyPress:
        {
            auto it = m_keybinds.find(evt.keyPress.keyCode);
            if (it != m_keybinds.end())
            {
                it->second();
                return false;
            }
        }
        case IEventType::WindowResize:
        {
            m_ui->doLayout();
            break;
        }
        default:
            break;
    }

    return inputImpl(evt);
}

void LevelController::update(std::uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    return updateImpl(ticks, iinter, rInter);
}

bool LevelController::hasNextController() const
{
    return m_nextController != std::shared_ptr<LevelController>();
}

std::shared_ptr<LevelController> LevelController::getNextController()
{
    return m_nextController;
}

bool LevelController::shouldPopController() const
{
    return m_shouldPopController;
}

void LevelController::popController()
{
    m_shouldPopController = true;
}

void LevelController::onExit()
{
    m_shouldPopController = false;
    m_nextController = std::shared_ptr<LevelController>();
    onExitImpl();
}

void LevelController::onEnter()
{
    m_shouldPopController = false;
    m_nextController = std::shared_ptr<LevelController>();
    onEnterImpl();
}


void LevelController::addKeybinding(SDL_Keycode key, std::function<void()> const& callback)
{
    m_keybinds.emplace(key, callback);
}


void LevelController::removeKeybinding(SDL_Keycode key)
{
    m_keybinds.erase(key);
}

void LevelController::onEnterImpl() { }
void LevelController::onExitImpl() { }
void LevelController::updateImpl(std::uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) {}

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

void LevelController::pushActionController(EntityRef ref, GameAction const& action)
{
    switch (action.ttype)
    {
        case TargetingType::SingleTile:
            pushController<ActionControllerSingleTile>(m_level, m_ui, ref, action);
            break;
        case TargetingType::SingleEntity:
            pushController<ActionControllerSingleEntity>(m_level, m_ui, ref, action);
            break;
        default:
            AssertNotImplemented();
    }
}



// Default Level Controller
// --------------------------------------


DefaultLController::DefaultLController(Level *level, UI::Manager* ui)
    : LevelController(level, ui)
{
    // Don't let us pop the top level controller
    removeKeybinding(SDLK_ESCAPE);

    addKeybinding( SDLK_ESCAPE, [this, level](){
        m_ui->toggleExclusiveDialog<UI::LevelMainMenu>( "levelMainMenu", level );
    });
}

bool DefaultLController::onMouseMove(IEventMouseMove evt)
{
    return false;
}

bool DefaultLController::onMouseDown(IEventMouseDown evt)
{
    if ( evt.button == SDL_BUTTON_LEFT )
    {
        auto tile = m_level->screenCoordsToTile(evt.screenPos);
        auto ents = m_level->grid().entitiesAtTile(tile);

        // Is there something where we clicked?
        if ( !ents.empty() )
        {
            auto actorEnt = m_level->ecs().firstEntityWith<ActorComponent>( ents );
            if ( actorEnt != EntityNull )
            {
                // Is the thing we clicked a player character?
                auto actorComp = m_level->ecs().getComponents<ActorComponent>( actorEnt );
                if ( actorComp->actorType == ActorType::PC )
                {
                    pushController<PlayerSelectedController>(m_level, m_ui, actorEnt);
                    return true;
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
    m_ui->removeSingleTileHighlight();

    if ( m_hoveredEntity != EntityNull )
    {
        m_hoveredEntity = EntityNull;
        m_level->events().broadcast<GameEvents::ControllerEntityHovered>( m_hoveredEntity );
    }

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
        m_ui->showSingleTileHighlight(m_level->tileCoordsToScreen(curr), UI::SingleTileHighlightType::Yellow);
        
        // Tell the UI that we've hovered an entity
        m_hoveredEntity = ents.back();
        m_level->events().broadcast<GameEvents::ControllerEntityHovered>( m_hoveredEntity );
    }
}

void DefaultLController::updateImpl(std::uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    scrollLevel(ticks, iinter);
}

void DefaultLController::onExitImpl()
{
    m_ui->removeSingleTileHighlight();
    m_lastHoveredTile = {-1, -1};
}




// Player Selected Controller
// --------------------------------------



PlayerSelectedController::PlayerSelectedController(Level* level, UI::Manager* ui, EntityRef entity)
        : LevelController(level, ui), m_entity(entity),
          m_defMoveAction(  "move", TargetingType::SingleTile, std::make_shared<ActionMoveStride>() ),
          m_defAttackAction( "strike", TargetingType::SingleEntity, std::make_shared<ActionMeleeAttack>() )
{
    // Hotkeys for the action menus
    addKeybinding( SDLK_q, [this](){
        m_ui->withId<UI::ActionMenu>("action-menu")->toggleMenu(RawActionDataType::Attack);
    });

    addKeybinding( SDLK_w, [this](){
        m_ui->withId<UI::ActionMenu>("action-menu")->toggleMenu(RawActionDataType::Move);
    });

    addKeybinding( SDLK_e, [this](){
        m_ui->withId<UI::ActionMenu>("action-menu")->toggleMenu(RawActionDataType::Item);
    });
}

void PlayerSelectedController::updateImpl(std::uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    scrollLevel(ticks, iinter);
}

bool PlayerSelectedController::onKeyDown(IEventKeyPress evt)
{
    return false;
}

bool PlayerSelectedController::onMouseDown(IEventMouseDown evt)
{
    auto tile = m_level->screenCoordsToTile(evt.screenPos);
    auto ents = m_level->grid().entitiesAtTile(tile);

    if ( evt.button == SDL_BUTTON_LEFT )
    {
        if ( ents.empty() )
        {
            popController();
        }
        else
        {
            auto actorEnt = m_level->ecs().firstEntityWith<ActorComponent>( ents );
            if ( actorEnt != EntityNull )
            {
                // Is the thing we clicked a player character?
                auto actorComp = m_level->ecs().getComponents<ActorComponent>( actorEnt );
                if ( actorComp->actorType == ActorType::PC )
                {
                    replaceController<PlayerSelectedController>(m_level, m_ui, actorEnt);
                    return true;
                }
            }
        }
    }
    else if ( evt.button == SDL_BUTTON_RIGHT )
    {
        // If we've right clicked on a valid target to default attack, perform the default attack.
        auto idx = std::find( m_defAttackTiles.begin(), m_defAttackTiles.end(), tile );
        if ( idx != m_defAttackTiles.end() )
        {
            auto actorEnt = m_level->ecs().firstEntityWith<ActorComponent>( ents );
            Assert( m_defAttackTargeting->entityIsValid(actorEnt) );
    
            m_defAttackTargeting->perform( actorEnt );
            m_level->events().broadcast<GameEvents::EntityAction>(m_entity, m_defAttackAction );
    
            popController();
            return true;
        }
    
        // If we've right clicked on a valid tile to basic move to, perform the move.
        idx = std::find( m_defMoveTiles.begin(), m_defMoveTiles.end(), tile );
        if ( idx != m_defMoveTiles.end() )
        {
            Assert( m_defMoveTargeting->tileIsValid( tile ));
    
            m_defMoveTargeting->perform( tile );
            m_level->events().broadcast<GameEvents::EntityAction>(m_entity, m_defMoveAction);
    
            popController();
            return true;
        }
    }
    
    // Otherwise, ignore the click for now.
    
    return false;
}

void PlayerSelectedController::onHoveredTileChange(Vector2i prev, Vector2i curr)
{
    // As we move over possible attack/move/examine targets, update our cursor to match

    auto idx = std::find( m_defAttackTiles.begin(), m_defAttackTiles.end(), curr );
    if ( idx != m_defAttackTiles.end() )
    {
        ResourceManager::get().getWindow()->cursor().setCursorType( CursorType::Attack );
        return;
    }
    
    idx = std::find( m_defMoveTiles.begin(), m_defMoveTiles.end(), curr );
    if ( idx != m_defMoveTiles.end() )
    {
        ResourceManager::get().getWindow()->cursor().setCursorType( CursorType::Move );
        return;
    }
    
    ResourceManager::get().getWindow()->cursor().resetCursor();
}

void PlayerSelectedController::onExitImpl()
{
    m_ui->removeSingleTileHighlight();
    m_level->events().broadcast<GameEvents::ControllerEntitySelected>(EntityNull);

    auto actorC = m_level->ecs().getComponents<ActorComponent>( m_entity );
    if ( actorC->actorType == ActorType::PC )
    {
        m_ui->withId( "ui-equip-view" )->hide();
        m_ui->withId( "player-inventory" )->hide();
    }
    
    m_ui->deleteElement(m_defaultMoveHighlight);
    m_ui->deleteElement(m_defaultAttackHighlight);
    
    ResourceManager::get().getWindow()->cursor().resetCursor();
}

void PlayerSelectedController::onEnterImpl()
{
    // Highlight the selected player
    auto position = m_level->ecs().getComponents<PositionComponent>(m_entity)->tilePosition;
    m_ui->showSingleTileHighlight(m_level->tileCoordsToScreen(position), UI::SingleTileHighlightType::Green);

    auto actorC = m_level->ecs().getComponents<ActorComponent>( m_entity );
    if ( actorC->actorType == ActorType::PC )
    {
        // Show the equip menu
        m_ui->withId( "ui-equip-view" )->show();
        m_ui->withId( "player-inventory" )->show();
    }
    
    
    // Add some default actions - basic move and basic attack

    
    if ( actorC->actor.canPerformAction( m_defAttackAction ) )
    {
        m_defAttackTargeting = std::static_pointer_cast<SingleEntityTargeting>(m_defAttackAction.impl);
        m_defAttackTargeting->attach( m_level, m_entity );
        m_defAttackTiles = m_defAttackTargeting->getValidTiles();
        
        if ( !m_defAttackTiles.empty() )
        {
            m_defaultAttackHighlight = m_ui->createElement<UI::TileRegionHighlight>(nullptr, m_defAttackTiles, Colour::Red);
        }
    }
    
    
    if ( actorC->actor.canPerformAction( m_defMoveAction ) )
    {
        m_defMoveTargeting = std::static_pointer_cast<SingleTileTargeting>(m_defMoveAction.impl);
        m_defMoveTargeting->attach( m_level, m_entity );
        m_defMoveTiles = m_defMoveTargeting->getValidTiles();
        
        if ( !m_defMoveTiles.empty() )
        {
            m_defaultMoveHighlight = m_ui->createElement<UI::TileRegionHighlight>(nullptr, m_defMoveTiles, Colour::Lime);
        }
    }
    

    m_level->events().broadcast<GameEvents::ControllerEntitySelected>(m_entity);
}


// Action Controllers
// --------------------------------------



// Single tile
// --------------------------------------


ActionControllerSingleTile::ActionControllerSingleTile(Level *level, UI::Manager* ui, EntityRef ref, GameAction const& action)
    : LevelController(level, ui),
      m_targeting{ std::static_pointer_cast<SingleTileTargeting>(m_action.impl) },
      m_action(action),
      m_entity(ref)
{
    m_targeting->attach(m_level, ref);

    // Highlight the valid tiles
    m_tileHighlight = m_ui->createElement<UI::TileRegionHighlight>(nullptr, m_targeting->getValidTiles(), Colour::Lime);
    m_origin = level->ecs().getComponents<PositionComponent>(ref)->tilePosition;
}

bool ActionControllerSingleTile::onMouseDown(IEventMouseDown evt)
{
    if ( evt.button == SDL_BUTTON_LEFT )
    {

    }
    else if ( evt.button == SDL_BUTTON_RIGHT )
    {
        auto tile = m_level->screenCoordsToTile(evt.screenPos);

        if ( m_targeting->tileIsValid(tile) )
        {
            m_targeting->perform(tile);
            m_level->events().broadcast<GameEvents::EntityAction>(m_entity, m_action);

            popController();
            return true;
        }
    }

    return false;
}

void ActionControllerSingleTile::onExitImpl()
{
    m_ui->deleteElement(m_tileHighlight);
    m_ui->deleteElement(m_pathHighlight);
}

void ActionControllerSingleTile::updateImpl(std::uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    scrollLevel(ticks, iinter);
}

void ActionControllerSingleTile::onHoveredTileChange(Vector2i prev, Vector2i curr)
{
    if ( m_targeting->isMovement() )
    {
        if ( m_targeting->tileIsValid(curr) )
        {
            m_tilePath = m_targeting->pathToTile(curr);
            m_ui->deleteElement(m_pathHighlight);
//            m_pathHighlight = m_ui->createElement<UI::TileRegionHighlight>(nullptr, m_tilePath, Colour::Red);
            m_pathHighlight = m_ui->createElement<UI::TileArrowHighlight>(nullptr, m_tilePath, Colour::Red, m_origin);
        }
    }
}


// Single Entity
// --------------------------------------


ActionControllerSingleEntity::ActionControllerSingleEntity(Level *level, UI::Manager* ui, EntityRef ref, const GameAction &action)
:   LevelController(level, ui),
    m_targeting{ std::static_pointer_cast<SingleEntityTargeting>(m_action.impl) },
    m_action(action),
    m_entity(ref)
{
    m_targeting->attach(m_level, ref);
}

bool ActionControllerSingleEntity::onMouseDown(IEventMouseDown evt)
{
    if ( evt.button == SDL_BUTTON_LEFT )
    {

    }
    else if ( evt.button == SDL_BUTTON_RIGHT )
    {
        auto tile = m_level->screenCoordsToTile(evt.screenPos);
        auto ents = m_level->grid().entitiesAtTile(tile);

        // TODO Disambiguate multiple ents per tile

        if ( ents.empty() )
        {
            return false;
        }

        EntityRef target = ents.front();

        if ( m_targeting->entityIsValid(target) )
        {
            m_targeting->perform(target);
            m_level->events().broadcast<GameEvents::EntityAction>(m_entity, m_action);

            popController();
            return true;
        }
    }

    return false;
}


void ActionControllerSingleEntity::onEnterImpl()
{
    m_tileHighlight = m_ui->createElement<UI::TileRegionHighlight>(
        nullptr, m_targeting->getValidTiles(), Colour::Lime);
}

void ActionControllerSingleEntity::onExitImpl()
{
    m_ui->deleteElement(m_tileHighlight);
}

void ActionControllerSingleEntity::updateImpl(std::uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    scrollLevel(ticks, iinter);
}

void ActionControllerSingleEntity::onHoveredTileChange(Vector2i prev, Vector2i curr)
{

}

