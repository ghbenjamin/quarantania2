#pragma once

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
};