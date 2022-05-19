#include <game/Combat.h>



void Damage::mergeDamage( Damage const &other )
{
    instances.insert( instances.end(), other.instances.begin(), other.instances.end() );
}
