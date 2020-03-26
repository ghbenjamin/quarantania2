#pragma once

#include <unordered_map>
#include <string_view>

#include <game/Items.h>
#include <game/Weapon.h>

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

    void setNaturalWeaponData( WeaponData* data );

    WeaponData* getCurrentWeaponData();

private:

    std::string m_name;

    WeaponData* m_naturalWeapon;
    ItemPtr m_equippedWeapon;
    std::unordered_map<EquipSlot, ItemPtr> m_equippedItems;
};