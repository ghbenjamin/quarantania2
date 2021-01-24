#pragma once


#include <game/Defines.h>
#include <game/Items.h>
#include <game/Action.h>

struct CreatureData;
struct PlayerData;
struct Damage;


class AbilityScore
{
public:
    AbilityScore(int value = 0);
    ~AbilityScore() = default;

    void setValue(int val);
    int getValue() const;
    int getMod() const;

private:
    int m_base;
    int m_mod;
};

class AbilityScoreBlock
{
public:
    AbilityScoreBlock();
    AbilityScoreBlock(int STR, int DEX, int CON, int INT, int WIS, int CHA);
    ~AbilityScoreBlock() = default;

    AbilityScore const& getScore(AbilityScoreType type) const;
    AbilityScore& getScore(AbilityScoreType type);

private:
    std::unordered_map<AbilityScoreType, AbilityScore> m_scores;
};



class Actor
{

public:

    Actor( PlayerData const& pdata );
    Actor( CreatureData const& data );
    ~Actor() = default;

    // Meta
    std::string const& getName() const;
    CreatureSize getSize();

    // Stats
    AbilityScoreBlock& abilityScores();

    // Items
    bool hasEquipped(ItemEquipSlot slot ) const;
    const ItemPtr getEquipped(ItemEquipSlot slot ) const;
    ItemPtr unequipItem(ItemEquipSlot slot );
    ItemPtr equipItem(ItemEquipSlot slot, ItemPtr item );

    // Weapons
    Weapon const& getActiveWeapon() const;
    Weapon const& getNaturalWeapon() const;
    float getReach() const;

    // Movement
    int getSpeed() const;

    // Health
    int getCurrentHp() const;
    int getMaxHp() const;
    void setCurrentHp( int value );

    // Armour
    int getAC() const;

private:

    // Info
    const std::string m_name;
    CreatureSize m_size;

    // Stats
    AbilityScoreBlock m_abilityScores;
    int m_baseSpeed;

    // Health
    int m_HpCurrent;
    int m_HpMax;

    // Items
    std::unordered_map<ItemEquipSlot, ItemPtr> m_equippedItems;
    std::vector<Weapon> m_naturalWeapons;
};