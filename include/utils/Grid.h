#pragma once

#include <unordered_map>
#include <utils/Containers.h>

using GridBitmask = std::uint8_t;
enum class Direction : GridBitmask
{
    N   = 0x01,
    NE  = 0x02,
    E   = 0x04,
    SE  = 0x08,
    S   = 0x10,
    SW  = 0x20,
    W   = 0x40,
    NW  = 0x80
};


Direction operator | (Direction lhs, Direction rhs);
Direction& operator |= (Direction& lhs, Direction rhs);
GridBitmask operator & (GridBitmask lhs, Direction rhs);
GridBitmask operator | (GridBitmask lhs, Direction rhs);
GridBitmask& operator|= (GridBitmask& lhs, Direction rhs);
bool operator== (GridBitmask& lhs, Direction rhs);


class Grid
{
public:
    static const GridBitmask CardinalOnly;

    static std::unordered_map<Direction, Vector2i> AllNeighbours;
    static std::unordered_map<Direction, Vector2i> CardinalNeighbours;

    static bool isAdjacent(Vector2i lhs, Vector2i rhs);
};

