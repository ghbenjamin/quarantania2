#pragma once

#include <game/GEvent.h>
#include <game/Entity.h>
#include <game/Items.h>


namespace GEvents
{

struct EntityMove : public GEvent<EntityMove>
{
    EntityMove(EntityRef ent, const Vector2i &oldPos, const Vector2i &newPos);
    ~EntityMove() override = default;

    EntityRef ent;
    Vector2i oldPos;
    Vector2i newPos;
};

struct EntityReady : public GEvent<EntityReady>
{
    explicit EntityReady(EntityRef ent);
    ~EntityReady() override = default;

    EntityRef ent;
};

struct EntityOpenClose : public GEvent<EntityOpenClose>
{
    EntityOpenClose(EntityRef ent, bool isOpen);
    ~EntityOpenClose() override = default;

    EntityRef ent;
    bool isOpen;
};

struct EntityUnlock : public GEvent<EntityUnlock>
{
    EntityRef ent;
};

struct LevelReady : public GEvent<LevelReady>
{
    LevelReady() = default;
    ~LevelReady() override = default;
};

struct GameTick : public GEvent<GameTick>
{
    GameTick() = default;
    ~GameTick() override = default;
};

struct MeleeAttack : public GEvent<MeleeAttack>
{
    MeleeAttack(EntityRef attacker, EntityRef defender, WeaponPtr const& weapon);
    ~MeleeAttack() override = default;

    EntityRef attacker;
    EntityRef defender;
    WeaponPtr weapon;
};

struct ItemPickup : public GEvent<ItemPickup>
{
    ItemPickup(EntityRef actor, EntityRef item);
    ~ItemPickup() override = default;

    EntityRef actor;
    EntityRef item;
};

struct ItemDrop : public GEvent<ItemDrop>
{
    ItemDrop(EntityRef actor, std::shared_ptr<Item> item);
    ~ItemDrop() override = default;

    EntityRef actor;
    std::shared_ptr<Item> item;
};

struct ItemEquip : public GEvent<ItemEquip>
{
    ItemEquip(EntityRef actor, std::shared_ptr<Item> item, EquipSlot slot);
    ~ItemEquip() override = default;

    EntityRef actor;
    std::shared_ptr<Item> item;
    EquipSlot slot;
};

struct ItemUnequip : public GEvent<ItemUnequip>
{
    ItemUnequip(EntityRef actor, EquipSlot slot);
    ~ItemUnequip() override = default;

    EntityRef actor;
    EquipSlot slot;
};

}
