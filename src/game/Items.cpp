#include <game/Items.h>
#include <utils/Json.h>
#include <utils/Logging.h>
#include <game/RawData.h>
#include <game/ResourceDatabase.h>


Weapon::Weapon(RawWeaponData const& rawData)
{
    initFromData( rawData );
}

void Weapon::initFromData(RawWeaponData const &rawData)
{
    m_name = rawData.itemName;
    m_critData = {rawData.critLower, rawData.critMult };
    m_baseDamage = rawData.damage;
    m_proficiency = rawData.proficiency;
}

std::shared_ptr<Weapon> Weapon::fromName(std::string_view name)
{
    RawWeaponData rawData = ResourceDatabase::instance().weaponFromName( name );
    return std::make_shared<Weapon>( rawData );
}

std::string const &Weapon::name() const
{
    return m_name;
}

WeaponHandedness Weapon::handedness() const
{
    return m_handedness;
}

WeaponType Weapon::type() const
{
    return m_weaponType;
}

WeaponProficiency Weapon::proficiency() const
{
    return m_proficiency;
}

CritData const &Weapon::critData() const
{
    return m_critData;
}

DiceRoll const &Weapon::damage() const
{
    return m_baseDamage;
}

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

Item::Item(RawItemData const &rawData)
{
    initFromData( rawData );
}

std::shared_ptr<Item> Item::fromName(std::string_view name)
{
    RawItemData rawData = ResourceDatabase::instance().itemFromName( name );
    return std::make_shared<Item>( rawData );
}

void Item::initFromData(RawItemData const &rawData)
{
    m_name = rawData.name;
    m_value = rawData.value;
    m_weight = rawData.weight;
    m_sprite = rawData.sprite;
    m_description = rawData.description;
    m_equipSlot = ResourceDatabase::parseEquipSlotFromStr( rawData.slot );

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
        m_weapon = Weapon::fromName( m_name );
    }
    else if (rawData.itemType == "armour" )
    {
        m_itemType = ItemType::Armour;
        m_armour = Armour::fromName( m_name );
    }
    else
    {
        m_itemType = ItemType::Unknown;
    }
}

EquipSlot Item::getEquipSlot() const
{
    return m_equipSlot;
}

bool Item::isWeapon() const
{
    return !!m_weapon;
}

WeaponPtr const &Item::getWeapon() const
{
    return m_weapon;
}

bool Item::isArmour() const
{
    return !!m_armour;
}

ArmourPtr const &Item::getArmour() const
{
    return m_armour;
}

std::string const& Item::getDescription() const
{
    return m_description;
}

UI::TooltipData Item::tooltipData() const
{
    UI::TooltipData td { m_name, "Item", m_description };

    auto fc = td.title.at(0);
    fc = std::toupper(fc);
    td.title[0] = fc;

    return td;
}

Armour::Armour(RawArmourData const &rawData)
{
    initFromData( rawData );
}

Armour::Armour(std::string_view name)
{
    RawArmourData rawData = ResourceDatabase::instance().armourFromName( name );
    initFromData( rawData );
}


std::shared_ptr<Armour> Armour::fromName(std::string_view name)
{
    RawArmourData rawData = ResourceDatabase::instance().armourFromName( name );
    return std::make_shared<Armour>( rawData );
}

void Armour::initFromData(RawArmourData const &rawData)
{
    m_arcaneFailureChance = rawData.arcaneFailureChance;
    m_shieldBonus = rawData.shieldBonus;
    m_armourBonus = rawData.armourBonus;
    m_maxDex = rawData.maxDex;
    m_speed20 = rawData.speed20;
    m_speed30 = rawData.speed30;
    m_armourCheck = rawData.armourCheck;
    m_armourType = ResourceDatabase::parseArmourTypeFromStr(rawData.armourType);
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
    return m_speed30 ;
}

