#pragma once

#include <vector>

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


struct AbilityScoreBonus
{
    AbilityScoreType type;
    int natural;
    int modified;
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

struct MovementSpeedData
{
    int natural = -1;
    int modified = -1;
    
    int moveSpeedLimit = -1;
    float multiplier = 1.0f;
};