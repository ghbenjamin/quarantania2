#pragma once



enum class Passibility
{
    Passable,
    Difficult,
    Impassable
};

enum class Visibility
{
    Hidden,
    Explored,
    Visible
};

using LightLevel = std::uint8_t;


struct DiceRoll
{
    int diceCount;
    int diceSize;
};

struct CritData
{
    int lowerRange;
    int multiplier;
};