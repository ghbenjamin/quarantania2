#include <game/Character.h>
#include <algorithm>

Character::Character()
{
}

Character::Character(const RawCreatureData& rcd)
{
    m_name = rcd.name;
}


std::string_view Character::name() const
{
    return m_name;
}

bool Character::hasEquipped(EquipSlot slot) const
{
    return m_equippedItems.find(slot) != m_equippedItems.end();
}

const ItemPtr Character::getEquipped(EquipSlot slot) const
{
    auto it = m_equippedItems.find(slot);
    if ( it != m_equippedItems.end() )
    {
        return it->second;
    }
    else
    {
        return ItemPtr();
    }
}

ItemPtr Character::unequipItem(EquipSlot slot)
{
    auto it = m_equippedItems.find(slot);
    auto ptr = it->second;

    m_equippedItems.erase(it);
    return ptr;
}

ItemPtr Character::equipItem(EquipSlot slot, ItemPtr item)
{
    auto lastEquipped = std::shared_ptr<Item>();

    if ( hasEquipped(slot) )
    {
        lastEquipped = unequipItem(slot);
    }

    m_equippedItems.emplace( slot, item );

    return lastEquipped;
}

