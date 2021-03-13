#pragma once

#include <vector>
#include <optional>

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

// A bonus to an Ability Score, e.g. STR or DEX. Always given as a raw number, not as a +- modifier
struct AbilityScoreBonus
{
    AbilityScoreType type;
    int natural;
    int modified;
};

// A bonus to a saving throw, e.g. REF or WILL.
struct SavingThrowRoll
{
    SavingThrowType type;
    int natural;
    int modified;
    
    EntityRef source;
    EntityRef defender;
};

// A single instance of a melee attack, e.g. one half of a double attack.
struct SingleMeleeAttackInstance
{
    EntityRef attackerRef;
    EntityRef defenderRef;
    
    Actor* attacker;
    Actor* defender;
    
    Weapon const* weapon;
};

// A single weapon - attack mod pair to help describe how many attacks an attacker gets for a given attack type
struct MeleeAttackCountItem
{
    MeleeAttackCountItem(Weapon const* weapon, int naturalAttackMod);
    
    Weapon const* weapon;
    int naturalAttackMod;
};

struct MeleeAttackCountData
{
    std::vector<MeleeAttackCountItem> attacks;
};

// A single attack roll to hit, made vs a single attacker.
struct AttackRoll
{
    SingleMeleeAttackInstance const* ctx;
    
    int naturalRoll = -1;
    int targetValue = -1;
    int modifiedRoll = -1;
    
    bool isHit = false;
    bool isCrit = false;
};

struct DamageRoll
{

};

struct MovementSpeedData
{
    int natural = -1;
    int modified = -1;
    
    int moveSpeedLimit = -1;
    float multiplier = 1.0f;
};

struct ArmourClassData
{
    int shieldBonus = 0;
    int armourBonus = 0;

    int dexBonus = 0;
    std::optional<int> maxDexToAc = {};

    int dodgeBonus = 0;
    int staticBonus = 10;

    int modifiedAC = 0;
};