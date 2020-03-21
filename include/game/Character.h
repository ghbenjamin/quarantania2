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

    /**
     *  Is there an item equipped in this slot?
     */
    bool hasEquipped( EquipSlot slot ) const;

    /**
     *   Return a (readonly) pointer to the item in the given slot, if one exists.
     */
    const ItemPtr getEquipped( EquipSlot slot ) const;

    /**
     *  Unequip the item in given slot, returning the item removed.
     *  If the given slot is empty, noop.
     */
    ItemPtr unequipItem( EquipSlot slot );

    /**
     *  Equip the given item in the given slot. If there is already an item in the given slot, unequip it
     *  first and return it.
     */
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