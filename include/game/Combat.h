#pragma once

#include <vector>
#include <optional>

#include <game/Defines.h>

struct DamageInstance
{
    DamageType type;
    DamageSuperType superType;
    int total;
};

struct Damage
{
    std::vector<DamageInstance> instances;
    
    void mergeDamage( Damage const& other );
};
