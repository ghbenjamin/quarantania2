#include <game/AttackDefs.h>
#include <game/ActorModifier.h>


// MeleeAttackPowerAttack
// ------------------------------------------------

void MeleeAttackPowerAttack::modifyAttackRoll( ActorCalcData& data )
{
    data.mods.addItem(ActorCalcOperation::Add, "Power Attack", -2);
}

void MeleeAttackPowerAttack::modifyDamageRoll( ActorCalcData& data )
{
    data.mods.addItem(ActorCalcOperation::Add, "Power Attack", 2);
}
