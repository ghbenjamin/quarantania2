#pragma once

#include <queue>

#include <game/Defines.h>
#include <game/Items.h>
#include <game/Action.h>
#include <game/Combat.h>
#include <game/ActorModifier.h>

struct CreatureData;
struct PlayerData;
struct Damage;
class Level;
class RandomInterface;

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

    Actor( Level* level, EntityRef ref, PlayerData const& pdata );
    Actor( Level* level, EntityRef ref, CreatureData const& data );
    ~Actor() = default;

    // Meta
    std::string const& getName() const;
    CreatureSize getSize();

    // Stats
    AbilityScoreBlock& abilityScores();

    // Items
    bool hasEquipped( CreatureEquipSlot slot ) const;
    const ItemPtr getEquipped( CreatureEquipSlot slot ) const;
    std::optional<CreatureEquipSlot> defaultSlotForItemSlot( ItemEquipSlot slot ) const;
    std::unordered_map<CreatureEquipSlot, ItemPtr> const& getAllEquippedItems() const;
    
    ItemPtr unequipItem( CreatureEquipSlot slot );
    ItemPtr equipItem( CreatureEquipSlot slot, ItemPtr item );

    // Weapons
    Weapon const& getActiveWeapon() const;
    Weapon const& getNaturalWeapon() const;
    float getReach() const;
    Armour const* tryGetActiveShield() const;

    // Movement
    int getSpeed() const;

    // Health
    int getCurrentHp() const;
    int getMaxHp() const;
    void setCurrentHp( int value );
    
    void acceptDamage( Damage const& dmg );
    
    // Armour
    int getAC() const;

    // Combat
    int getCritRangeForAttack( SingleAttackInstance& attack ) const;
    Damage getDamageForAttack( SingleAttackInstance& attack, AttackRollResult const& roll ) const;
    int getAcForDefender( SingleAttackInstance& attack ) const;
    AttackRollResult makeAttackRoll( SingleAttackInstance& attack, bool isCritConfirm ) const;
    
    // Modifiers
    void addModifier( ActorMod const& mod );
    

private:

    // Meta
    EntityRef m_entity;
    Level* m_level;

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
    std::unordered_map<CreatureEquipSlot, ItemPtr> m_equippedItems;
    std::vector<Weapon> m_naturalWeapons;

    // Modifiers
    std::priority_queue<ActorMod, std::queue<ActorMod>, ActorModCompare> m_modifiers;
    
};