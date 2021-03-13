#pragma once

#include <game/Attack.h>


class MeleeAttackStandard : public MeleeAttack
{
    // Normal behaviour, no modifications
};

class MeleeAttackFullAttack : public MeleeAttack
{
    // Additional attacks with weapons in exchange for no movement
};


class MeleeAttackPowerAttack : public MeleeAttack
{
    // Exchange attack bonus for damage
public:
    void modifyAttackRoll( AttackRoll &roll ) override;
    void modifyDamageRoll( DamageRoll &roll ) override;
};


class MeleeAttackVitalStrike : public MeleeAttack
{
    // Make a single attack with increased damage
public:
    void modifyDamageRoll( DamageRoll &roll ) override;
};