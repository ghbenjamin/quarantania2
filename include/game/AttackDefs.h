#pragma once

#include <game/Attack.h>

// Normal behaviour, no modifications
class MeleeAttackStandard : public MeleeAttack {};

// Additional attacks with weapons in exchange for no movement
class MeleeAttackFullAttack : public MeleeAttack {};

// Exchange attack bonus for damage
class MeleeAttackPowerAttack : public MeleeAttack
{
public:
    void modifyAttackRoll( ActorCalcData& data ) override;
    void modifyDamageRoll( ActorCalcData& data ) override;
};
