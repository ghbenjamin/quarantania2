#include <game/Character.h>
#include <algorithm>

Character::Character()
: m_equippedWeapon(nullptr)
{
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
    return m_equippedItems.at(slot);
}

ItemPtr Character::unequipItem(EquipSlot slot)
{
    auto it = m_equippedItems.find(slot);
    auto ptr = it->second;

    m_equippedItems.erase(it);
    return ptr;
}

bool Character::hasEquippedWeapon() const
{
    return !!m_equippedWeapon;
}
ItemPtr Character::equippedWeaponData() const
{
    return m_equippedWeapon;
}

ItemPtr Character::unequipWeapon()
{
    auto ptr = m_equippedWeapon;
    m_equippedWeapon = ItemPtr {};
    return ptr;
}

void Character::equipItem(EquipSlot slot, ItemPtr item)
{
    m_equippedItems.emplace( slot, item );
}

void Character::equipWeapon(ItemPtr item)
{
    m_equippedWeapon = item;
}

WeaponData *Character::getCurrentWeaponData()
{
    if ( m_equippedWeapon )
    {
        // foo
    }
    else if ( m_naturalWeapon != nullptr )
    {
        return m_naturalWeapon;
    }

    return nullptr;
}

void Character::setNaturalWeaponData( WeaponData* data )
{
    m_naturalWeapon = data;
}

