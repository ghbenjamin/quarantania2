#pragma once

#include <engine/Entity.h>

enum class DamageType
{
    Physical,
    Elemental,
    Force,
    Energy,
    Alignment,
    Poison,
    Mental,
    Nonlethal,
    Precision
};

enum class DamageSubType
{
    Untyped
    // Magical
    // Physical
};

struct Damage
{
    int total;

    DamageType type;
    DamageSubType subType;

    EntityRef source;
};