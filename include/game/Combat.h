#pragma once

#include <engine/Entity.h>

class Actor;
class Weapon;

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