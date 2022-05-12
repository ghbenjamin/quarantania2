#include <game/Items.h>
#include <game/RawData.h>
#include <game/ResourceDatabase.h>
#include <game/GameParse.h>


// Weapons
// --------------------------------------

Weapon::Weapon(WeaponData const& rawData)
    :   m_critRange(rawData.critLower),
        m_critMultiplier(rawData.critMult),
        m_baseDamage(rawData.damage),
        m_weaponType(rawData.weaponType)
{
}

Weapon Weapon::fromId( std::string_view id )
{
    WeaponData rawData = ResourceDatabase::get().weaponFromId(id);
    return Weapon( rawData );
}

WeaponHandedness Weapon::handedness() const
{
    return m_handedness;
}

WeaponType Weapon::type() const
{
    return m_weaponType;
}

DiceRoll const &Weapon::damage() const
{
    return m_baseDamage;
}

int Weapon::critMultiplier() const
{
    return m_critMultiplier;
}

int Weapon::critRange() const
{
    return m_critRange;
}


const Weapon WeaponUnarmedAttack = { WeaponData {
    "Unarmed",
    DiceRoll{1, 3},
    20,
    2,
    WeaponType::Melee
}};


// Items
// --------------------------------------

const std::string &Item::getName() const
{
    return m_name;
}

ItemType Item::getType() const
{
    return m_itemType;
}

int Item::getValue() const
{
    return m_value;
}

int Item::getWeight() const
{
    return m_weight;
}

const SpritesheetKey &Item::getSprite() const
{
    return m_sprite;
}

Item::Item(ItemData const &rawData)
{
    initFromData( rawData );
}

std::shared_ptr<Item> Item::fromName(std::string_view name)
{
    ItemData rawData = ResourceDatabase::get().itemFromId(name);
    return std::make_shared<Item>( rawData );
}

void Item::initFromData(ItemData const &rawData)
{
    m_id = rawData.id;
    m_name = rawData.name;
    m_value = rawData.value;
    m_weight = rawData.weight;
    m_sprite = rawData.sprite;
    m_description = rawData.description;
    m_equipSlot = EnumParse::equipSlot(rawData.slot);

    if (rawData.itemType == "armour" )
    {
        m_itemType = ItemType::Armour;
    }
    else if (rawData.itemType == "consumable" )
    {
        m_itemType = ItemType::Consumable;
    }
    else if (rawData.itemType == "equippable" )
    {
        m_itemType = ItemType::Equippable;
    }
    else if (rawData.itemType == "gear" )
    {
        m_itemType = ItemType::Gear;
    }
    else if (rawData.itemType == "ammo" )
    {
        m_itemType = ItemType::Ammo;
    }
    else if (rawData.itemType == "weapon" )
    {
        m_itemType = ItemType::Weapon;
        m_weapon = Weapon::fromId( m_id );
    }
    else if (rawData.itemType == "armour" )
    {
        m_itemType = ItemType::Armour;
        m_armour = Armour::fromId( m_id );
    }
    else
    {
        m_itemType = ItemType::Unknown;
    }
}

ItemEquipSlot Item::getEquipSlot() const
{
    return m_equipSlot;
}

bool Item::isWeapon() const
{
    return !!m_weapon;
}

Weapon const &Item::getWeapon() const
{
    Assert( m_weapon.has_value() );
    return m_weapon.value();
}

bool Item::isArmour() const
{
    return !!m_armour;
}

Armour const &Item::getArmour() const
{
    Assert( m_weapon.has_value() );
    return m_armour.value();
}

std::string const& Item::getDescription() const
{
    return m_description;
}

// Armour
// --------------------------------------

Armour::Armour(ArmourData const &rawData)
{
    initFromData( rawData );
}

Armour Armour::fromId( std::string_view id )
{
    ArmourData rawData = ResourceDatabase::get().armourFromId(id);
    return Armour( rawData );
}

void Armour::initFromData(ArmourData const &rawData)
{
    m_arcaneFailureChance = rawData.arcaneFailureChance;
    m_shieldBonus = rawData.shieldBonus;
    m_armourBonus = rawData.armourBonus;
    m_maxDex = rawData.maxDex;
    m_speed20 = rawData.speed20;
    m_speed30 = rawData.speed30;
    m_armourCheck = rawData.armourCheck;
    m_armourType = EnumParse::armourType(rawData.armourType);
}

ArmourType Armour::armourType() const
{
    return m_armourType;
}

int Armour::arcaneFailureChance() const
{
    return m_arcaneFailureChance;
}

int Armour::armourBonus() const
{
    return m_armourBonus;
}

int Armour::shieldBonus() const
{
    return m_shieldBonus;
}

int Armour::armourCheck() const
{
    return m_armourCheck;
}

int Armour::maxDex() const
{
    return m_maxDex;
}

int Armour::speed20() const
{
    return m_speed20;
}

int Armour::speed30() const
{
    return m_speed30;
}