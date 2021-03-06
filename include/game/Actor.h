#pragma once

#include <queue>
#include <map>

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
class MeleeAttack;


class ModifiableRollVisitor
{
public:
    ModifiableRollVisitor( Actor const* actor );
    ~ModifiableRollVisitor() = default;
    
    void operator()( AttackRoll* roll );
    void operator()( DamageRoll* roll );
    void operator()( SavingThrowRoll* roll );
    void operator()( AbilityScoreBonus* roll );
    void operator()( MovementSpeedData* data );
    void operator()( ArmourClassData* data );
    void operator()( ActionSpeedData* data );
    void operator()( MeleeAttackCountData* data );

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
    std::string const& getCreatureType() const;

    // Stats
    int getAbilityScoreValue( AbilityScoreType type ) const;
    int getAbilityScoreMod( AbilityScoreType type ) const;
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
    std::optional<CreatureEquipSlot> canEquipItem( ItemPtr item );
    
    ItemPtr unequipItem( CreatureEquipSlot slot );
    ItemPtr equipItem( CreatureEquipSlot slot, ItemPtr item );

    // Weapons
    
    std::pair<Weapon const*, Weapon const*> getEquippedWeapons() const;
    Weapon const& getNaturalWeapon() const;
    float getReach() const;
    Armour const* tryGetActiveShield() const;

    // Movement
    int getSpeed() const;

    // Health
    int getCurrentHp() const;
    int getMaxHp() const;
    HealthLevel getHealthLevel() const;
    void setCurrentHp( int value );
    void acceptDamage( Damage const& dmg );
    
    
    // Defense
    int getAC() const;
    SavingThrowRoll makeSavingThrow( EntityRef source, SavingThrowType type ) const;

    // Combat
    int getCritRangeForAttack( SingleMeleeAttackInstance& attack ) const;
    MeleeAttackCountData getAttackCountForMeleeAttack( std::shared_ptr<MeleeAttack> attackImpl ) const;
    Damage makeMeleeDamageRoll( SingleMeleeAttackInstance& attack, std::shared_ptr<MeleeAttack> attackImpl, AttackRoll const& roll ) const;
    int getAcForMeleeAttack( SingleMeleeAttackInstance& attack, std::shared_ptr<MeleeAttack> attackImpl ) const;
    AttackRoll makeMeleeAttackRoll( SingleMeleeAttackInstance& attack, std::shared_ptr<MeleeAttack> attackImpl, bool isCritConfirm ) const;

    // Modifiers
    void addModifierGroup( ActorModGroup const& mod );
    void removeActorModGroup( std::string const& id );
    void applyAllModifiers(ModifiableStatObject roll ) const;
    
    template <typename T>
    void modifyTypedRoll(ActorStatModType type, T* roll ) const
    {
        auto range = m_statModifiers.equal_range(type);
        for ( auto it = range.first; it != range.second; it++ )
        {
            auto ptr = std::static_pointer_cast<ActorStatModImpl<T>>(it->second.impl);
            ptr->modify(roll);
        }
    }
    
    // Actions
    std::vector<GameAction> getAllGameActions() const;
    ActionsUsedInfo& actionInfo();
    ActionsUsedInfo const& actionInfo() const;
    bool canPerformAction( GameAction const& action ) const;

private:

    // Meta
    EntityRef m_entity;
    Level* m_level;

    // Info
    const std::string m_name;
    CreatureSize m_size;
    const std::string m_creatureType;

    // Stats
    int m_baseAbilityScoreStr;
    int m_baseAbilityScoreDex;
    int m_baseAbilityScoreCon;
    int m_baseAbilityScoreInt;
    int m_baseAbilityScoreWis;
    int m_baseAbilityScoreCha;
    int m_baseSpeed;

    // Health
    int m_HpCurrent;
    int m_HpMax;

    // Items
    std::unordered_map<CreatureEquipSlot, ItemPtr> m_equippedItems;
    std::vector<Weapon> m_naturalWeapons;

    // Modifiers
    std::vector<ActorModGroup> m_modifierGroups;
    std::multimap<ActorStatModType, ActorStatMod> m_statModifiers;
    std::vector<ActorActionMod> m_actionMods;
    
    // Actions
    ActionsUsedInfo m_actionInfo;
};