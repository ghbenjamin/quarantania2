#pragma once

#include <optional>
#include <variant>

#include <game/GameEvent.h>
#include <engine/Entity.h>
#include <game/Items.h>
#include <game/Combat.h>
#include <game/Action.h>

struct Damage;
class MeleeAttack;

namespace GameEvents
{

/**
 *  Add any new events to GameEventVariant.
 */


// Movement Events
// ------------------------------


struct EntityMove : public GameEvent<EntityMove>
{
    EntityMove(EntityRef ent, const Vector2i &oldPos, const Vector2i &newPos);
    EntityMove(EntityRef ent, const Vector2i &oldPos, const Vector2i &newPos, std::vector<Vector2i> const& path);
    ~EntityMove() override = default;

    EntityRef ent;
    Vector2i oldPos;
    Vector2i newPos;
    std::optional<std::vector<Vector2i>> path;
};

struct EntityReady : public GameEvent<EntityReady>
{
    explicit EntityReady(EntityRef ent);
    ~EntityReady() override = default;

    EntityRef ent;
};

struct EntityOpenClose : public GameEvent<EntityOpenClose>
{
    EntityOpenClose(EntityRef ent, bool isOpen);
    ~EntityOpenClose() override = default;

    EntityRef ent;
    bool isOpen;
};

struct LevelReady : public GameEvent<LevelReady>
{
    LevelReady() = default;
    ~LevelReady() override = default;
};

struct TurnChange : public GameEvent<TurnChange>
{
    TurnChange(bool isPlayerTurn);
    ~TurnChange() override = default;

    bool isPlayerTurn;
};

struct RoundChange : public GameEvent<RoundChange> {};

struct EntityAction : public GameEvent<EntityAction>
{
    EntityAction(EntityRef entity, GameAction speed);
    ~EntityAction() override = default;

    EntityRef entity;
    GameAction speed;
};


struct ItemPickup : public GameEvent<ItemPickup>
{
    ItemPickup(EntityRef actor, EntityRef item);
    ~ItemPickup() override = default;

    EntityRef actor;
    EntityRef item;
};

struct ItemDrop : public GameEvent<ItemDrop>
{
    ItemDrop(EntityRef actor, std::shared_ptr<Item> item);
    ~ItemDrop() override = default;

    EntityRef actor;
    std::shared_ptr<Item> item;
};

struct ItemEquip : public GameEvent<ItemEquip>
{
    ItemEquip(EntityRef actor, std::shared_ptr<Item> item, CreatureEquipSlot slot);
    ~ItemEquip() override = default;

    EntityRef actor;
    std::shared_ptr<Item> item;
    CreatureEquipSlot slot;
};

struct ItemUnequip : public GameEvent<ItemUnequip>
{
    ItemUnequip(EntityRef actor, CreatureEquipSlot slot);
    ~ItemUnequip() override = default;

    EntityRef actor;
    CreatureEquipSlot slot;
};

struct EntityDeath : public GameEvent<EntityDeath>
{
    EntityDeath(EntityRef actor);
    ~EntityDeath() override = default;

    EntityRef actor;
};

struct EntityDamage : public GameEvent<EntityDamage>
{
    EntityDamage(EntityRef target, EntityRef source, Damage const& dmg );
    ~EntityDamage() override = default;

    EntityRef target;
    EntityRef source;
    Damage damage;
};


// Combat events
// ------------------------------



struct CombatMeleeAttack : public GameEvent<CombatMeleeAttack>
{
    CombatMeleeAttack(EntityRef attacker, EntityRef defender, std::shared_ptr<MeleeAttack> attack);
    ~CombatMeleeAttack() override = default;

    EntityRef attacker;
    EntityRef defender;
    std::shared_ptr<MeleeAttack> attack;
};


struct CombatAttackSucceeded : public GameEvent<CombatAttackSucceeded>
{
    CombatAttackSucceeded(EntityRef attacker, EntityRef defender);
    ~CombatAttackSucceeded() override = default;
    
    EntityRef attacker;
    EntityRef defender;
};


struct CombatMissedAttack : public GameEvent<CombatMissedAttack>
{
    CombatMissedAttack(EntityRef attacker, EntityRef defender);
    ~CombatMissedAttack() override = default;

    EntityRef attacker;
    EntityRef defender;
};


// Controller Events
// ------------------------------

struct ControllerEntitySelected : public GameEvent<ControllerEntitySelected>
{
    ControllerEntitySelected(EntityRef entity);
    ~ControllerEntitySelected() override = default;

    EntityRef entity;
};

struct ControllerEntityHovered : public GameEvent<ControllerEntitySelected>
{
    ControllerEntityHovered(EntityRef entity);
    ~ControllerEntityHovered() override = default;
    
    EntityRef entity;
};


// Overworld Events
// ------------------------------

struct OverworldLocationSelect : public GameEvent<OverworldLocationSelect>
{
    OverworldLocationSelect( int locationIdx );
    ~OverworldLocationSelect() override = default;
    
    int locationIdx;
};


}