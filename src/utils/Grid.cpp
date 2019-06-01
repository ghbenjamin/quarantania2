#include <utils/Grid.h>

std::unordered_map<Direction, Vector2i> Grid::AllNeighbours = {
        { Direction::N, {0, -1} },
        { Direction::NE, {1, -1} },
        { Direction::E, {1, 0} },
        { Direction::SE, {1, 1} },
        { Direction::S, {0, 1} },
        { Direction::SW, {-1, 1} },
        { Direction::W, {-1, 0} },
        { Direction::NW, {-1, -1} },
};

std::unordered_map<Direction, Vector2i> Grid::CardinalNeighbours = {
        { Direction::N, {0, -1} },
        { Direction::E, {1, 0} },
        { Direction::S, {0, 1} },
        { Direction::W, {-1, 0} },
};

const GridBitmask Grid::CardinalOnly = (GridBitmask)( Direction::N | Direction::E | Direction::W | Direction::S );

bool Grid::isAdjacent(Vector2i lhs, Vector2i rhs)
{
    int x = std::abs(rhs.x() - lhs.x());
    int y = std::abs(rhs.y() - lhs.y());

    return ( x == 0 && y < 2) || ( y == 0 && x < 2 );
}

Direction operator | (Direction lhs, Direction rhs)
{
    return static_cast<Direction>(static_cast<GridBitmask>(lhs) | static_cast<GridBitmask>(rhs));
}

Direction& operator |= (Direction& lhs, Direction rhs)
{
    lhs = lhs | rhs;
    return lhs;
}

GridBitmask operator & (GridBitmask lhs, Direction rhs)
{
    return lhs & static_cast<GridBitmask>(rhs);
}

GridBitmask operator | (GridBitmask lhs, Direction rhs)
{
    return lhs | static_cast<GridBitmask>(rhs);
}

GridBitmask& operator|= (GridBitmask& lhs, Direction rhs)
{
    lhs = lhs | static_cast<GridBitmask>(rhs);
    return lhs;
}


bool operator== (GridBitmask& lhs, Direction rhs)
{
    return lhs == static_cast<GridBitmask>(rhs);
}