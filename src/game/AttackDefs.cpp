#include <game/AttackDefs.h>
#include <game/ActorModifier.h>

void MeleeAttackPowerAttack::modifyAttackRoll( ActorCalc::AttackRoll &roll )
{
    roll.modList().addItem(ActorCalcOperation::Add, -1);
}

void MeleeAttackPowerAttack::modifyDamageRoll( ActorCalc::DamageRoll &roll )
{

}
