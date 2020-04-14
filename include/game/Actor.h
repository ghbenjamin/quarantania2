#pragma once

#include <unordered_map>
#include <string_view>

#include <game/Items.h>

struct RawCreatureData;
struct PlayerData;

class Actor
{
public:

    Actor(PlayerData const& pdata);
    Actor(RawCreatureData const& rcd);
    ~Actor() = default;

    // Getters
    std::string_view name() const;

    // Items
    bool hasEquipped( EquipSlot slot ) const;
    const ItemPtr getEquipped( EquipSlot slot ) const;
    ItemPtr unequipItem( EquipSlot slot );
    ItemPtr equipItem( EquipSlot slot, ItemPtr item );

    // Weapons
    WeaponPtr getActiveWeapon() const;
    WeaponPtr getNaturalWeapon() const;


private:
    // Info
    std::string m_name;

    // Items
    std::unordered_map<EquipSlot, ItemPtr> m_equippedItems;
    std::vector<WeaponPtr> m_naturalWeapons;

};