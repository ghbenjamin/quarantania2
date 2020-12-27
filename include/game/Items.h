#pragma once

#include <memory>
#include <optional>
#include <unordered_map>

#include <resource/Spritesheet.h>
#include <ui/Defines.h>
#include <game/Defines.h>
#include <game/Damage.h>

struct ItemData;
struct WeaponData;
struct ArmourData;

class Armour
{
public:
    Armour( ArmourData const& rawData );
    ~Armour() = default;

    static Armour fromName( std::string_view name );

    ArmourType armourType( ) const;
    int arcaneFailureChance( ) const;
    int armourBonus( ) const;
    int shieldBonus( ) const;
    int armourCheck( ) const;
    int maxDex( ) const;
    int speed20( ) const;
    int speed30( ) const;

private:
    void initFromData(ArmourData const& rawData );

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


class Weapon
{
public:
    Weapon( WeaponData const& rawData );
    ~Weapon() = default;

    static Weapon fromName( std::string_view name );

    WeaponHandedness handedness() const;
    WeaponType type() const;
    WeaponProficiency proficiency() const;
    CritData const& critData() const;
    DiceRoll const& damage() const;
    float getReach() const;

private:
    void initFromData( WeaponData const& rawData );

private:
    WeaponHandedness m_handedness;
    WeaponType m_weaponType;
    WeaponProficiency m_proficiency;
    CritData m_critData;
    DiceRoll m_baseDamage;
    PhysDamageMask m_damageType;
    WeaponSpecialMask m_specials;
};


const extern Weapon WeaponUnarmedAttack;



class Item
{
public:
    Item( ItemData const& rawData );
    virtual ~Item() = default;

    static std::shared_ptr<Item> fromName( std::string_view name );

    const std::string &getName() const;
    ItemType getType() const;
    int getValue() const;
    int getWeight() const;
    EquipSlot getEquipSlot() const;
    const SpritesheetKey &getSprite() const;
    std::string const& getDescription() const;

    bool isWeapon() const;
    Weapon const& getWeapon() const;

    bool isArmour() const;
    Armour const& getArmour() const;

    UI::TooltipData tooltipData() const;


private:
    void initFromData(ItemData const& rawData );

private:
    std::string m_name;
    ItemType m_itemType;
    int m_value;
    int m_weight;
    SpritesheetKey m_sprite;
    std::string m_description;
    EquipSlot m_equipSlot;

    std::optional<Weapon> m_weapon;
    std::optional<Armour> m_armour;
};

using ItemPtr = std::shared_ptr<Item>;