#include <game/Combat.h>

MeleeAttackCountItem::MeleeAttackCountItem( Weapon const *weapon, int naturalAttackMod )
 : weapon(weapon), naturalAttackMod(naturalAttackMod) {}

void Damage::mergeDamage( Damage const &other )
{
    instances.insert( instances.end(), other.instances.begin(), other.instances.end() );
}
