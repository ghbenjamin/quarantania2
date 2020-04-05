#pragma once

#include <unordered_map>
#include <string_view>

#include <game/Items.h>


class Character
{
public:

    Character();
    ~Character() = default;

    std::string_view name() const;

    bool hasEquipped( EquipSlot slot ) const;
    const ItemPtr getEquipped( EquipSlot slot ) const;
    ItemPtr unequipItem( EquipSlot slot );
    ItemPtr equipItem( EquipSlot slot, ItemPtr item );

    bool hasEquippedWeapon() const;
    ItemPtr equippedWeaponData() const;


private:

    std::string m_name;

    ItemPtr m_equippedWeapon;
    std::unordered_map<EquipSlot, ItemPtr> m_equippedItems;
};