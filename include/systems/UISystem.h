#pragma once

#include <systems/System.h>
#include <game/GameEvent.h>
#include <game/GameEventDefs.h>
#include <resource/Sprite.h>

class UISystem : public System, public GameEventSub<UISystem>
{
public:
    explicit UISystem(Level *parent);
    ~UISystem() override = default;

    void operator()(GameEvents::LevelReady& evt);
    void operator()(GameEvents::EntityMove& evt);
    void operator()(GameEvents::ItemPickup& evt);
    void operator()(GameEvents::ItemDrop& evt);
    void operator()(GameEvents::ItemEquip& evt);
    void operator()(GameEvents::ItemUnequip& evt);
    void operator()(GameEvents::TurnChange& evt);
    void operator()(GameEvents::RoundChange& evt);
    void operator()(GameEvents::CombatMeleeAttack& evt);
    void operator()(GameEvents::EntityDamage& evt);
    void operator()(GameEvents::ControllerEntitySelected& evt);
    void operator()(GameEvents::EntityAction& evt);
    void operator()(GameEvents::CombatMissedAttack& evt);
    void operator()(GameEvents::CombatAttackSucceeded& evt);

    template <typename T>
    void operator()(T&& t)
    {}


private:

    void refreshPlayerInventory();
    void refreshPlayerEquipView();
    void refreshPartyStatus();

    void pushLogLine( std::string const& line, Colour const& colour = Colour::Black );
};