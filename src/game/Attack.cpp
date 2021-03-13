#include <game/Attack.h>

void MeleeAttack::modifyAttackRoll( AttackRoll &roll ) {}
void MeleeAttack::modifyDamageRoll( DamageRoll &roll ) {}
void MeleeAttack::onHit() {}

bool MeleeAttack::isFullAttack()
{
    return true;
}

