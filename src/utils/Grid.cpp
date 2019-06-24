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

GridRegion Grid::createLine(Vector2i start, Vector2i end)
{
    // Bresenham line algorithm

    int x0 = start.x();
    int y0 = start.y();
    int x1 = end.x();
    int y1 = end.y();

    if ( std::abs(y1 - y0) < std::abs(x1 - x0) )
    {
        if ( x0 > x1 )
            return Grid::createLineLow(x1, y1, x0, y0);
        else
            return Grid::createLineLow(x0, y0, x1, y1);
    }
    else
    {
        if ( y0 > y1 )
            return Grid::createLineHigh(x1, y1, x0, y0);
        else
            return Grid::createLineHigh(x0, y0, x1, y1);
    }
}

GridRegion Grid::createLineLow(int x0, int y0, int x1, int y1)
{
    GridRegion gr;

    int dx = x1 - x0;
    int dy = y1 - y0;
    int yi = 1;

    if (dy < 0)
    {
        yi = -1;
        dy = -dy;
    }

    int D = 2 * dy - dx;
    int y = y0;

    for ( int x = x0; x <= x1; x++ )
    {
        gr.push_back({x, y});

        if ( D > 0 )
        {
            y = y + yi;
            D = D - 2 * dx;
        }
        D = D + 2 * dy;
    }

    return gr;
}

GridRegion Grid::createLineHigh(int x0, int y0, int x1, int y1)
{
    GridRegion gr;

    int dx = x1 - x0;
    int dy = y1 - y0;
    int xi = 1;

    if (dx < 0)
    {
        xi = -1;
        dx = -dx;
    }

    int D = 2 * dx - dy;
    int x = x0;

    for ( int y = y0; y <= y1; y++ )
    {
        gr.push_back({x, y});

        if ( D > 0 )
        {
            x = x + xi;
            D = D - 2 * dy;
        }
        D = D + 2 * dx;
    }

    return gr;
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