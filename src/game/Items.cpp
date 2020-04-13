#include <game/Items.h>
#include <utils/Json.h>
#include <utils/Logging.h>
#include <db/RawData.h>
#include <db/ResourceDatabase.h>


Weapon::Weapon(RawWeaponData const& rawData)
{
    initFromData( rawData );
}

void Weapon::initFromData(RawWeaponData const &rawData)
{
    m_name = rawData.item_name;
    m_critData = { rawData.crit_lower, rawData.crit_mult };
    m_baseDamage = rawData.damage;
    m_proficiency = rawData.proficiency;
}

std::unique_ptr<Weapon> Weapon::fromName(std::string_view name)
{
    RawWeaponData rawData = ResourceDatabase::instance().weaponFromName( name );
    return std::make_unique<Weapon>( rawData );
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
    m_equipSlot = equipSlotFromString( rawData.slot );

    if ( rawData.item_type == "armour" )
    {
        m_itemType = ItemType::Armour;
    }
    else if ( rawData.item_type == "consumable" )
    {
        m_itemType = ItemType::Consumable;
    }
    else if ( rawData.item_type == "equippable" )
    {
        m_itemType = ItemType::Equippable;
    }
    else if ( rawData.item_type == "gear" )
    {
        m_itemType = ItemType::Gear;
    }
    else if ( rawData.item_type == "ammo" )
    {
        m_itemType = ItemType::Ammo;
    }
    else if ( rawData.item_type == "weapon" )
    {
        m_itemType = ItemType::Weapon;
        m_weapon = Weapon::fromName( m_name );
    }
    else if ( rawData.item_type == "armour" )
    {
        m_itemType = ItemType::Armour;
        m_armour = Armour::fromName( m_name );
    }
    else
    {
        m_itemType = ItemType::Unknown;
    }
}

EquipSlot Item::equipSlotFromString(std::string_view str)
{
    if ( str == "armor" )
    {
        return EquipSlot::Armor;
    }
    else if ( str == "arms" )
    {
        return EquipSlot::Arms;
    }
    else if ( str == "belt" )
    {
        return EquipSlot::Belt;
    }
    else if ( str == "body" )
    {
        return EquipSlot::Body;
    }
    else if ( str == "chest" )
    {
        return EquipSlot::Chest;
    }
    else if ( str == "eyes" )
    {
        return EquipSlot::Eyes;
    }
    else if ( str == "feet" )
    {
        return EquipSlot::Feet;
    }
    else if ( str == "hands" )
    {
        return EquipSlot::Hands;
    }
    else if ( str == "head" )
    {
        return EquipSlot::Head;
    }
    else if ( str == "headband" )
    {
        return EquipSlot::Headband;
    }
    else if ( str == "neck" )
    {
        return EquipSlot::Neck;
    }
    else if ( str == "ring" )
    {
        return EquipSlot::Ring;
    }
    else if ( str == "shield" )
    {
        return EquipSlot::Shield;
    }
    else if ( str == "shoulders" )
    {
        return EquipSlot::Shoulders;
    }
    else if ( str == "wrists" )
    {
        return EquipSlot::Wrists;
    }
    else if ( str == "weapon" )
    {
        return EquipSlot::Weapon;
    }
    else
    {
        return EquipSlot::None;
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

std::unique_ptr<Weapon> const &Item::getWeapon() const
{
    return m_weapon;
}

bool Item::isArmour() const
{
    return !!m_armour;
}

std::unique_ptr<Armour> const &Item::getArmour() const
{
    return m_armour;
}

std::string_view Item::getDescription() const
{
    return m_description;
}

UI::TooltipData Item::tooltipData() const
{
    UI::TooltipData td;
    td.title = m_name;
    td.subtitle = "Item";
    td.content = m_description;

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


std::unique_ptr<Armour> Armour::fromName(std::string_view name)
{
    RawArmourData rawData = ResourceDatabase::instance().armourFromName( name );
    return std::make_unique<Armour>( rawData );
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

    if ( rawData.armour_type == "Light armor" )
    {
        m_armourType = ArmourType::Light;
    }
    else if ( rawData.armour_type == "Medium armor" )
    {
        m_armourType = ArmourType::Medium;
    }
    else if ( rawData.armour_type == "Heavy armor" )
    {
        m_armourType = ArmourType::Heavy;
    }
    if ( rawData.armour_type == "Shields" )
    {
        m_armourType = ArmourType::Shield;
    }
    else
    {
        AssertAlwaysMsg( "Unknown armour type" );
    }
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

