#pragma once

#include <unordered_map>
#include <utils/Containers.h>

enum class Direction
{
    N,
    NE,
    E,
    SE,
    S,
    SW,
    W,
    NW
};

enum class CardinalDirection
{
    N,
    E,
    S,
    W
};

using GridBitmask = int8_t;

class Grid
{
public:
    static std::unordered_map<Direction, Vector2i> AllNeighbours;
    static std::unordered_map<CardinalDirection, Vector2i> CardinalNeighbours;

    static bool isAdjacent(Vector2i lhs, Vector2i rhs);
};

