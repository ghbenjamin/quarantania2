#pragma once

#include <optional>
#include <variant>

#include <game/GameEvent.h>
#include <engine/Entity.h>
#include <game/Items.h>

struct Damage;

namespace GameEvents
{


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
    TurnChange(EntityRef prev, EntityRef curr);
    ~TurnChange() override = default;

    EntityRef previous;
    EntityRef current;
};

struct RoundChange : public GameEvent<RoundChange>
{
};

struct EntityAction : public GameEvent<EntityAction>
{
    EntityAction(EntityRef entity, int actionPoints);
    ~EntityAction() override = default;

    EntityRef entity;
    int actionPoints;
};

//struct MeleeAttack : public GameEvent<MeleeAttack>
//{
//    MeleeAttack(EntityRef attacker, EntityRef defender, WeaponPtr const& weapon);
//    ~MeleeAttack() override = default;
//
//    EntityRef attacker;
//    EntityRef defender;
//    WeaponPtr weapon;
//};

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
    EntityDamage(EntityRef target, Damage const* dmg );
    ~EntityDamage() override = default;

    EntityRef target;
    Damage const* damage;
};

// Combat events
// ------------------------------


struct CombatMeleeAttack : public GameEvent<CombatMeleeAttack>
{
    CombatMeleeAttack(EntityRef attacker, EntityRef defender);
    ~CombatMeleeAttack() override = default;

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

}