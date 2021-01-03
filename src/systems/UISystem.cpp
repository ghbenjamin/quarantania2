#include <systems/UISystem.h>
#include <game/Level.h>
#include <ui/LevelUi.h>
#include <components/ActorComponent.h>

UISystem::UISystem(Level *parent)
        : System(parent)
{
    m_level->events().subscribe<GameEvents::LevelReady>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::EntityMove>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::ItemPickup>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::ItemDrop>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::ItemEquip>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::ItemUnequip>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::TurnChange>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::ControllerEntitySelected>(this, GEventTiming::After );
}

void UISystem::accept(GameEvents::LevelReady *evt)
{
    auto turnOrder = m_level->ui().withId<UI::TurnOrderContainer>( "turn-order-container" );
    turnOrder->reloadEntities();
}

void UISystem::accept(GameEvents::EntityMove *evt)
{

}

void UISystem::accept(GameEvents::ItemPickup *evt)
{
    
}

void UISystem::accept(GameEvents::ItemDrop *evt)
{
    
}

void UISystem::accept(GameEvents::ItemEquip *evt)
{
    
}

void UISystem::accept(GameEvents::ItemUnequip *evt)
{
    
}

void UISystem::accept(GameEvents::TurnChange *evt)
{
    auto turnOrder = m_level->ui().withId<UI::TurnOrderContainer>( "turn-order-container" );
    turnOrder->refresh();
}

void UISystem::accept(GameEvents::ControllerEntitySelected *evt)
{
    // We've selected an entity - update our UI to match
    auto actionMenu = m_level->ui().withId<UI::ActionMenu>( "action-menu" );
    actionMenu->closeMenu();
    actionMenu->refresh(evt->entity);
}
