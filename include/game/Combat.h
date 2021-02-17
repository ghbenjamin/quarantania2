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

struct SingleAttackInstance
{
    EntityRef attackerRef;
    EntityRef defenderRef;
    
    Actor* attacker;
    Actor* defender;
    
    Weapon const* weapon;
};



struct AttackRollResult
{
    int naturalRoll = -1;
    int targetValue = -1;
    int modifiedRoll = -1;
    
    bool isHit = false;
    bool isCrit = false;
};