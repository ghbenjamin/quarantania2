#pragma once

#include <memory>
#include <unordered_map>
#include <set>

#include <resource/Spritesheet.h>
#include <ui/Defines.h>
#include <game/Defines.h>

struct RawItemData;
struct RawWeaponData;
struct RawArmourData;
struct RawCreatureData;

enum class ItemType
{
    Unknown,
    Armour,
    Consumable,
    Equippable,
    Gear,
    Weapon,
    Ammo,
};

enum class EquipSlot
{
    None,
    Armor,
    Arms,
    Belt,
    Body,
    Chest,
    Eyes,
    Feet,
    Hands,
    Head,
    Headband,
    Neck,
    Ring,
    Shield,
    Shoulders,
    Weapon,
    Wrists,
};


enum class WeaponHandedness
{
    OneHanded,
    TwoHanded,
};

enum class WeaponType
{
    Melee,
    Ranged,
    Ammo
};

enum class WeaponProficiency
{
    Simple,
    Martial,
    Exotic
};

enum class PhysDamageType
{
    Piercing = 0x1,
    Bludgeoning = 0x2,
    Slashing = 0x4
};

enum class WeaponSpecials
{
    Trip,
    Reach,
    Nonlethal,
    Double,
    Disarm,
    Brace,
};

using PhysDamageMask = std::uint8_t;

enum class ArmourType
{
    Heavy,
    Medium,
    Light,
    Shield
};


class Armour
{
public:
    Armour( RawArmourData const& rawData );
    Armour( std::string_view name );
    ~Armour() = default;

    static std::unique_ptr<Armour> fromName( std::string_view name );

    ArmourType armourType( ) const;

    int arcaneFailureChance( ) const;
    int armourBonus( ) const;
    int shieldBonus( ) const;
    int armourCheck( ) const;
    int maxDex( ) const;
    int speed20( ) const;
    int speed30( ) const;

private:
    void initFromData( RawArmourData const& rawData );

private:
    int m_arcaneFailureChance;
    int m_armourBonus;
    int m_shieldBonus;
    int m_armourCheck;
    int m_maxDex;
    int m_speed20;
    int m_speed30;
    ArmourType m_armourType;
};

using ArmourPtr = std::unique_ptr<Armour>;


class Weapon
{
public:
    Weapon( RawWeaponData const& rawData );
    ~Weapon() = default;

    static std::unique_ptr<Weapon> fromName( std::string_view name );


private:
    void initFromData( RawWeaponData const& rawData );

private:
    std::string m_name;
    WeaponHandedness m_handedness;
    WeaponType m_weaponType;
    WeaponProficiency m_proficiency;
    CritData m_critData;
    DiceRoll m_baseDamage;
    PhysDamageMask m_damageType;
    std::set<WeaponSpecials> m_specials;
};

using WeaponPtr = std::unique_ptr<Weapon>;

class Item
{
public:
    Item( RawItemData const& rawData );
    virtual ~Item() = default;

    static std::shared_ptr<Item> fromName( std::string_view name );


    const std::string &getName() const;
    ItemType getType() const;
    int getValue() const;
    int getWeight() const;
    EquipSlot getEquipSlot() const;
    const SpritesheetKey &getSprite() const;
    std::string_view getDescription() const;

    bool isWeapon() const;
    std::unique_ptr<Weapon> const& getWeapon() const;

    bool isArmour() const;
    std::unique_ptr<Armour> const& getArmour() const;

    UI::TooltipData tooltipData() const;


private:
    void initFromData( RawItemData const& rawData );
    static EquipSlot equipSlotFromString( std::string_view str );

private:
    std::string m_name;
    ItemType m_itemType;
    int m_value;
    int m_weight;
    SpritesheetKey m_sprite;
    std::string m_description;
    EquipSlot m_equipSlot;

    WeaponPtr m_weapon;
    ArmourPtr m_armour;
};

using ItemPtr = std::shared_ptr<Item>;