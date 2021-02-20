#pragma once

#include <queue>

#include <game/Defines.h>
#include <game/Items.h>
#include <game/Action.h>
#include <game/Combat.h>
#include <game/ActorModifier.h>
#include <map>

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

class ModifiableRollVisitor
{
public:
    ModifiableRollVisitor( Actor const* actor );
    ~ModifiableRollVisitor() = default;
    
    void operator()( AttackRoll* roll );
    void operator()( SavingThrowRoll* roll );

private:
    Actor const* m_actor;
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
    int getModStr() const;
    int getModDex() const;
    int getModCon() const;
    int getModInt() const;
    int getModWis() const;
    int getModCha() const;

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
    
    // Defense
    int getAC() const;
    SavingThrowRoll makeSavingThrow( EntityRef source, SavingThrowType type ) const;

    // Combat
    int getCritRangeForAttack( SingleAttackInstance& attack ) const;
    Damage getDamageForAttack( SingleAttackInstance& attack, AttackRoll const& roll ) const;
    int getAcForDefender( SingleAttackInstance& attack ) const;
    AttackRoll makeAttackRoll( SingleAttackInstance& attack, bool isCritConfirm ) const;
    
    // Modifiers
    void addModifierGroup( ActorModGroup const& mod );
    void applyAllModifiers(ModifiableObject roll ) const;
    
    template <typename T>
    void modifyTypedRoll( ActorModType type, T* roll ) const
    {
        auto range = m_modifiers.equal_range(type);
        for ( auto it = range.first; it != range.second; it++ )
        {
            auto ptr = std::static_pointer_cast<ActorModImpl<T>>(it->second.impl);
            ptr->modify(roll);
        }
    }
    

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
    std::vector<ActorModGroup> m_modifierGroups;
    std::multimap<ActorModType, ActorMod> m_modifiers;
    
};