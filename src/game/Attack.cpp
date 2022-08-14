#include <game/Attack.h>

void MeleeAttack::modifyAttackRoll( ActorCalcData& data ) {}
void MeleeAttack::modifyDamageRoll( ActorCalcData& data ) {}
void MeleeAttack::onHit() {}

bool MeleeAttack::isFullAttack()
{
    return true;
}

