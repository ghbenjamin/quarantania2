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
class RandomState;
class MeleeAttack;


class Actor
{

public:
    Actor( Level* level, EntityRef ref, ActorData const& data );
    ~Actor() = default;

    // Meta
    EntityRef getRef() const;
    std::string const& getName() const;
    CreatureSize getSize();
    std::string const& getCreatureType() const;

    // Stats
    int getAbilityScoreValue( AbilityScoreType type ) const;
    int getAbilityScoreMod( AbilityScoreType type ) const;


    // Items
    bool hasEquipped( CreatureEquipSlot slot ) const;
    ItemPtr getEquipped( CreatureEquipSlot slot ) const;
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
    int getSpeed();

    // Health
    int getCurrentHp() const;
    int getMaxHp() const;
    HealthLevel getHealthLevel() const;
    void setCurrentHp( int value );
    void acceptDamage( Damage const& dmg );
    

    // Modifiers
    // ---------------------
    
    ActorCalcData getModifiersCritRange( Actor* defender, const Weapon* weapon ) const;
    ActorCalcData getModifiersSavingThrow( SavingThrowType type, EntityRef source ) const;
    ActorCalcData getModifiersMeleeDamage( ActorCalc::DamageRoll const& ctx, std::shared_ptr<MeleeAttack> attackImpl ) const;
    ActorCalcData getModifiersAttackRoll( Actor* defender, const Weapon* weapon, std::shared_ptr<MeleeAttack> attackImpl ) const;
    ActorCalcData getModifiersMovementSpeed() const;
    ActorCalcData getModifiersActionSpeed( const GameAction* action ) const;
    
    
    void addModifierGroup( ActorModGroup const& mod );
    void removeActorModGroup( std::string const& id );
    std::vector<ActorModGroup> const& getAllModifierGroups() const;
    
    
    // Actions
    // -------------------------
    
    // Return all the game actions this actor is allowed to take
    std::vector<GameAction> getAllGameActions() const;
    
    // This actor peform the specified action?
    bool canPerformAction( GameAction const& action ) const;
    
    // How many actions does this actor have remaining this round?
    int getActionsRemaining() const;
    
    // How many actions total can this entity perform by default?
    int getMaxActions() const;
    
    // Mark this actor as having used [count] actions from their available pool
    void useActions( int count );
    
    // Reset the available action count back to the maximum
    void resetActions();

private:

    // Accept an actor calculation data object and apply to it all of our modifiers which are applicable
    void applyAllModifiers( ActorCalcData& data ) const;
    

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

    // Saving throws
    int m_baseFortitude;
    int m_baseReflex;
    int m_baseWill;

    // Health
    int m_HpCurrent;
    int m_HpMax;

    // Items
    std::unordered_map<CreatureEquipSlot, ItemPtr> m_equippedItems;
    std::vector<Weapon> m_naturalWeapons;

    // Modifiers
    std::vector<ActorModGroup> m_modifierGroups;
    std::multimap<ActorCalculationType, ActorDynamicMod> m_dynamicModifiers;
    std::vector<ActorActionMod> m_actionMods;
    
    // Actions
    int m_maxActions;
    int m_usedActions;
};