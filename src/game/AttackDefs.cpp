#include <game/AttackDefs.h>
#include <game/ActorModifier.h>

void MeleeAttackPowerAttack::modifyAttackRoll( AttackRoll &roll )
{
    roll.ctx.finalRoll -= 1;
}

void MeleeAttackPowerAttack::modifyDamageRoll( DamageRoll &roll )
{

}
