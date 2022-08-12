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
    void modifyAttackRoll( ActorCalc::AttackRoll &roll ) override;
    void modifyDamageRoll( ActorCalc::DamageRoll &roll ) override;
};
