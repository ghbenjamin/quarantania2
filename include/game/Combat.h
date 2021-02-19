#pragma once

#include <engine/Entity.h>
#include <game/Defines.h>


class Actor;
class Weapon;


struct DamageInstance
{
    DamageType type;
    DamageSuperType superType;
    int total;
};

struct Damage
{
    std::vector<DamageInstance> instances;
};


struct SavingThrowRoll
{
    SavingThrowType type;
    int natural;
    int modified;
    
    EntityRef source;
    EntityRef defender;
};


struct SingleAttackInstance
{
    EntityRef attackerRef;
    EntityRef defenderRef;
    
    Actor* attacker;
    Actor* defender;
    
    Weapon const* weapon;
};


struct AttackRoll
{
    SingleAttackInstance const* ctx;
    
    
    int naturalRoll = -1;
    int targetValue = -1;
    int modifiedRoll = -1;
    
    bool isHit = false;
    bool isCrit = false;
};

