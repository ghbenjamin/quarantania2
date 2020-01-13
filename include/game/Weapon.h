#pragma once

#include <unordered_map>
#include <game/DiceRoll.h>

enum class WeaponHandedness
{
    OneHanded,
    TwoHanded,
};

enum class WeaponType
{
    Simple,
    Martial,
    Exotic
};

struct CritData
{
    int minCritRange = 20;
    int maxCritRange = 20;
    int critMultiplier = 2;
};

enum class PhysDamageType
{
    Piercing = 0x1,
    Bludgeoning = 0x2,
    Slashing = 0x4
};

using PhysDamageMask = std::uint8_t;

struct WeaponData
{
    std::string name;
    WeaponHandedness handedness;
    WeaponType type;
    int value;
    DiceRoll damage;
    CritData criticals;
    int reach;
    int weight;
    PhysDamageMask damageTypes;
};


class WeaponManager
{
public:
    WeaponManager() = default;
    ~WeaponManager() = default;

    void loadAllData();

private:

    static CritData parseCritLine( std::string const& data );

    static const inline std::regex CritRegex { "((\\d+)-(\\d+)\\/)?x(\\d+)" };
    std::unordered_map<std::string, const WeaponData> m_weaponData;
};