#pragma once

#include <string>
#include <regex>

struct DiceRoll
{
    int diceCount = 0;
    int diceSize = 0;
    int modifier = 0;

    static DiceRoll parseFromString( std::string const& data );

private:
    static const inline std::regex DiceRollRegex { "(\\d+)d(\\d+)([+-])?(\\d+)?" };
};