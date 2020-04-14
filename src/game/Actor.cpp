#include <game/Actor.h>
#include <algorithm>
#include <game/Player.h>
#include <db/RawData.h>


Actor::Actor(const RawCreatureData& rcd)
{
    m_name = rcd.name;
}


Actor::Actor(PlayerData const& pdata)
{
    m_name = pdata.name;
}

std::string_view Actor::name() const
{
    return m_name;
}

bool Actor::hasEquipped(EquipSlot slot) const
{
    return m_equippedItems.find(slot) != m_equippedItems.end();
}

const ItemPtr Actor::getEquipped(EquipSlot slot) const
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

ItemPtr Actor::unequipItem(EquipSlot slot)
{
    auto it = m_equippedItems.find(slot);
    auto ptr = it->second;

    m_equippedItems.erase(it);
    return ptr;
}

ItemPtr Actor::equipItem(EquipSlot slot, ItemPtr item)
{
    auto lastEquipped = std::shared_ptr<Item>();

    if ( hasEquipped(slot) )
    {
        lastEquipped = unequipItem(slot);
    }

    m_equippedItems.emplace( slot, item );

    return lastEquipped;
}

WeaponPtr Actor::getActiveWeapon() const
{
    auto it = m_equippedItems.find( EquipSlot::Weapon );
    if ( it != m_equippedItems.end() )
    {
        return it->second->getWeapon();
    }
    else
    {
        // Might return a null ptr
        return getNaturalWeapon();
    }
}

WeaponPtr Actor::getNaturalWeapon() const
{
    if ( m_naturalWeapons.empty() )
    {
        return WeaponPtr();
    }

    // TODO Don't just return the front weapon
    return m_naturalWeapons.front();
}

