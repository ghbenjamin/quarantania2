#include <utils/Grid.h>

std::unordered_map<Direction, Vector2i> Grid::AllNeighbours = {
        { Direction::N, {0, 1} },
        { Direction::NE, {1, 1} },
        { Direction::E, {1, 0} },
        { Direction::SE, {1, -1} },
        { Direction::S, {0, -1} },
        { Direction::SW, {-1, -1} },
        { Direction::W, {-1, 0} },
        { Direction::NW, {-1, 1} },
};

std::unordered_map<CardinalDirection, Vector2i> Grid::CardinalNeighbours = {
        { CardinalDirection::N, {0, 1} },
        { CardinalDirection::E, {1, 0} },
        { CardinalDirection::S, {0, -1} },
        { CardinalDirection::W, {-1, 0} },
};

bool Grid::isAdjacent(Vector2i lhs, Vector2i rhs)
{
    int x = std::abs(rhs.x() - lhs.x());
    int y = std::abs(rhs.y() - lhs.y());

    return ( x == 0 && y < 2) || ( y == 0 && x < 2 );
}
