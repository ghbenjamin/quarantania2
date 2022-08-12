#include <game/Attack.h>

void MeleeAttack::modifyAttackRoll( ActorCalc::AttackRoll &roll ) {}
void MeleeAttack::modifyDamageRoll( ActorCalc::DamageRoll &roll ) {}
void MeleeAttack::onHit() {}

bool MeleeAttack::isFullAttack()
{
    return true;
}

