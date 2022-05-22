#include <game/AttackDefs.h>
#include <game/ActorModifier.h>

void MeleeAttackPowerAttack::modifyAttackRoll( AttackRoll &roll )
{
    roll.addModComponent( ModComponentType::Add, -1 );
}

void MeleeAttackPowerAttack::modifyDamageRoll( DamageRoll &roll )
{

}
