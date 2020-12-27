#pragma once


enum class DamageType
{
    Untyped,
    Elemental,
    Force,
    Energy,
    Alignment,
    Poison,
    Mental,
    Nonlethal,
    Precision
};

enum class ElementalDamageType
{
    Fire,
    Cold,
    Electricity,
    Acid
};

enum class DamageSuperType
{
     Magical,
     Physical
};

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