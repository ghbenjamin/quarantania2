#pragma once

#include <variant>

namespace GameEvents
{
    struct EntityMove;
    struct EntityReady;
    struct LevelReady;
    struct TurnChange;
    struct RoundChange;
    struct EntityAction;
    struct ItemPickup;
    struct ItemDrop;
    struct ItemEquip;
    struct ItemUnequip;
    struct EntityDeath;
    struct EntityDamage;
    struct CombatMeleeAttack;
    struct CombatMissedAttack;
    struct ControllerEntitySelected;
    struct ControllerEntityHovered;
    struct CombatAttackSucceeded;
    struct OverworldLocationSelect;
}

using GameEventVariant = std::variant<
        GameEvents::EntityMove,
        GameEvents::EntityReady,
        GameEvents::LevelReady,
        GameEvents::TurnChange,
        GameEvents::RoundChange,
        GameEvents::EntityAction,
        GameEvents::ItemPickup,
        GameEvents::ItemDrop,
        GameEvents::ItemEquip,
        GameEvents::ItemUnequip,
        GameEvents::EntityDeath,
        GameEvents::EntityDamage,
        GameEvents::CombatMeleeAttack,
        GameEvents::CombatMissedAttack,
        GameEvents::ControllerEntitySelected,
        GameEvents::ControllerEntityHovered,
        GameEvents::CombatAttackSucceeded,
        GameEvents::OverworldLocationSelect
>;