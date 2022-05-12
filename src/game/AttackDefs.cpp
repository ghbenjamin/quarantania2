#include <game/AttackDefs.h>
#include <game/Combat.h>

void MeleeAttackPowerAttack::modifyAttackRoll( AttackRoll &roll )
{
    roll.modifiedRoll -= 1;
}

void MeleeAttackPowerAttack::modifyDamageRoll( DamageRoll &roll )
{

}
