#pragma once

#include <systems/System.h>

#include <game/GameEvent.h>
#include <game/GameEventDefs.h>
#include <game/Combat.h>

class Actor;


class CombatSystem : public System, public GameEventSub<CombatSystem>
{
public:
    explicit CombatSystem(Level* parent);
    ~CombatSystem() override = default;

    void operator()(GameEvents::CombatMeleeAttack& evt);

    template <typename T>
    void operator()(T&& t)
    {}

private:
    
    
    // Perform a single melee attack by one entity against another entity
    void doMeleeAttack( EntityRef attacker, EntityRef defender );
    
    
    
    void acceptDamage( Damage const& dmg, EntityRef ref );

    // Roll a single attack roll. Calculate whether or not it hits, and whether or not it is a crit.
    AttackRollResult makeAttackRoll( SingleAttackInstance& attack, bool isCritConfirm );

    // Calculate the to-hit value for a specific attack instance between two actors
    int getAcForDefender( SingleAttackInstance& attack );

    // Determine the crit range for a specific attack instance
    int getCritRangeForAttack( SingleAttackInstance& attack );

    // Calculate how much
    Damage getDamageForAttack( SingleAttackInstance& attack, AttackRollResult const& roll );

};