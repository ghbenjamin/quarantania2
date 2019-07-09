#pragma once


namespace Rules
{

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


}

