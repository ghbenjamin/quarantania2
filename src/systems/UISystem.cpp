#include <systems/UISystem.h>
#include <game/Level.h>
#include <components/ActorComponent.h>

#include <ui/level/PlayerStatusView.h>
#include <ui/level/ActionPopupMenu.h>
#include <ui/level/CreatureEquipView.h>
#include <ui/level/ContainerView.h>
#include <ui/level/EntityInformationView.h>
#include <ui/level/BetterTextLog.h>


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
    m_level->events().subscribe<GameEvents::RoundChange>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::ControllerEntitySelected>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::ControllerEntityHovered>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::CombatMeleeAttack>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::EntityAction>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::CombatAttackSucceeded>(this, GEventTiming::After );
    m_level->events().subscribe<GameEvents::CombatMissedAttack>(this, GEventTiming::After );
}

void UISystem::operator()(GameEvents::LevelReady& evt)
{
    auto playerStatus = m_level->ui().withId<UI::PlayerStatusContainer>( "player-status-container" );
    playerStatus->reloadEntities();
}

void UISystem::operator()(GameEvents::EntityMove& evt)
{

}

void UISystem::operator()(GameEvents::ItemPickup& evt)
{
    
}

void UISystem::operator()(GameEvents::ItemDrop& evt)
{
    
}

void UISystem::operator()(GameEvents::ItemEquip& evt)
{
    auto creatureEquip = m_level->ui().withId<UI::EquipView>( "ui-equip-view" );
    creatureEquip->refresh(evt.actor);

    auto creatureInventory = m_level->ui().withId<UI::ContainerView>( "player-inventory" );
    creatureInventory->refresh(evt.actor);
}

void UISystem::operator()(GameEvents::ItemUnequip& evt)
{
    auto creatureEquip = m_level->ui().withId<UI::EquipView>( "ui-equip-view" );
    creatureEquip->refresh(evt.actor);

    auto creatureInventory = m_level->ui().withId<UI::ContainerView>( "player-inventory" );
    creatureInventory->refresh(evt.actor);
}

void UISystem::operator()(GameEvents::TurnChange& evt)
{
    auto playerStatus = m_level->ui().withId<UI::PlayerStatusContainer>( "player-status-container" );
    playerStatus->refresh();
}

void UISystem::operator()(GameEvents::ControllerEntitySelected& evt)
{
    // We've selected an entity - update our UI to match
    auto actionMenu = m_level->ui().withId<UI::ActionMenu>( "action-menu" );
    actionMenu->closeMenu();
    actionMenu->refresh(evt.entity);

    auto creatureEquip = m_level->ui().withId<UI::EquipView>( "ui-equip-view" );
    creatureEquip->refresh(evt.entity);

    auto creatureInventory = m_level->ui().withId<UI::ContainerView>( "player-inventory" );
    creatureInventory->refresh(evt.entity);
}

void UISystem::operator()(GameEvents::CombatMeleeAttack &evt)
{
    pushLogLine( fmt::format(
        "{} attacks {}",
        m_level->getDescriptionForEnt( evt.attacker ),
        m_level->getDescriptionForEnt( evt.defender )
    ));
}

void UISystem::pushLogLine(const std::string &line, const Colour &colour)
{
    auto textLog = m_level->ui().withId<UI::BetterTextLog>("main-text-log");
    textLog->addLine(line, colour);
}

void UISystem::operator()( GameEvents::EntityDamage &evt )
{
    // TODO: Use real value not unmodified value
    int total = 0;
    for (auto const& d : evt.damage.instances )
    {
        total += d.total;
    }

    pushLogLine( fmt::format(
            "{} deals {} damage to {}",
            m_level->getDescriptionForEnt( evt.source ),
            total,
            m_level->getDescriptionForEnt( evt.target )
    ));
}

void UISystem::operator()(GameEvents::EntityAction &evt)
{
    refreshPartyStatus();
}

void UISystem::operator()(GameEvents::RoundChange &evt)
{
    refreshPartyStatus();

    m_level->ui().displayTransientMessage("Player Turn");
}

void UISystem::operator()(GameEvents::CombatMissedAttack &evt)
{
    pushLogLine( fmt::format(
            "{} misses {} ",
            m_level->getDescriptionForEnt( evt.attacker ),
            m_level->getDescriptionForEnt( evt.defender )
    ));
}

void UISystem::operator()( GameEvents::CombatAttackSucceeded &evt )
{
    pushLogLine( fmt::format(
            "{} hits {} ",
            m_level->getDescriptionForEnt( evt.attacker ),
            m_level->getDescriptionForEnt( evt.defender )
    ));
}

void UISystem::refreshPlayerInventory()
{

}

void UISystem::refreshPlayerEquipView()
{

}

void UISystem::refreshPartyStatus()
{
    auto playerStatus = m_level->ui().withId<UI::PlayerStatusContainer>( "player-status-container" );
    playerStatus->reloadEntities();
}

void UISystem::operator()( GameEvents::ControllerEntityHovered &evt )
{
    auto entityInformation = m_level->ui().withId<UI::EntityInformationView>("entity-information-view");
    entityInformation->refresh(evt.entity);
    
    if (evt.entity == EntityNull)
    {
        entityInformation->hide();
    }
    else
    {
        entityInformation->show();
    }
}
