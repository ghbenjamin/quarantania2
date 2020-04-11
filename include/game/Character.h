#pragma once

#include <unordered_map>
#include <string_view>

#include <game/Items.h>

struct RawCreatureData;

class Character
{
public:

    Character();
    Character(RawCreatureData const& rcd);
    ~Character() = default;

    std::string_view name() const;

    bool hasEquipped( EquipSlot slot ) const;
    const ItemPtr getEquipped( EquipSlot slot ) const;
    ItemPtr unequipItem( EquipSlot slot );
    ItemPtr equipItem( EquipSlot slot, ItemPtr item );

private:
    std::string m_name;
    std::unordered_map<EquipSlot, ItemPtr> m_equippedItems;
};