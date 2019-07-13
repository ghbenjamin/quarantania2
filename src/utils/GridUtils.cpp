#include <utils/GridUtils.h>
#include <utils/Logging.h>

std::unordered_map<Direction, Vector2i> GridUtils::AllNeighbours = {
        { Direction::N, {0, -1} },
        { Direction::NE, {1, -1} },
        { Direction::E, {1, 0} },
        { Direction::SE, {1, 1} },
        { Direction::S, {0, 1} },
        { Direction::SW, {-1, 1} },
        { Direction::W, {-1, 0} },
        { Direction::NW, {-1, -1} },
};

std::unordered_map<Direction, Vector2i> GridUtils::CardinalNeighbours = {
        { Direction::N, {0, -1} },
        { Direction::E, {1, 0} },
        { Direction::S, {0, 1} },
        { Direction::W, {-1, 0} },
};

const GridBitmask GridUtils::CardinalOnly = (GridBitmask)( Direction::N | Direction::E | Direction::W | Direction::S );

bool GridUtils::isAdjacent(Vector2i lhs, Vector2i rhs)
{
    int x = std::abs(rhs.x() - lhs.x());
    int y = std::abs(rhs.y() - lhs.y());

    return ( x == 0 && y < 2) || ( y == 0 && x < 2 );
}

GridRegion GridUtils::createLine(Vector2i start, Vector2i end)
{
    // Bresenham line algorithm

    int x0 = start.x();
    int y0 = start.y();
    int x1 = end.x();
    int y1 = end.y();

    if ( std::abs(y1 - y0) < std::abs(x1 - x0) )
    {
        if ( x0 > x1 )
            return GridUtils::createLineLow(x1, y1, x0, y0);
        else
            return GridUtils::createLineLow(x0, y0, x1, y1);
    }
    else
    {
        if ( y0 > y1 )
            return GridUtils::createLineHigh(x1, y1, x0, y0);
        else
            return GridUtils::createLineHigh(x0, y0, x1, y1);
    }
}

GridRegion GridUtils::createLineLow(int x0, int y0, int x1, int y1)
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

    return std::move(gr);
}

GridRegion GridUtils::createLineHigh(int x0, int y0, int x1, int y1)
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

    return std::move(gr);
}

GridRegion GridUtils::createRect(Vector2i origin, Vector2i size)
{
    GridRegion gr;
    Vector2i end = origin + size;

    for ( int y = origin.y(); y < end.y(); y++ )
    {
        for ( int x = origin.x(); x < end.x(); x++ )
        {
            gr.push_back({x, y});
        }
    }

    return std::move(gr);
}

GridRegion GridUtils::createCircle(Vector2i origin, int radius)
{
    GridRegion gr;
    int r2 = radius * radius;
    Vector2i tl = origin - Vector2i{radius, radius};
    Vector2i br = { tl.x() + 2 * radius + 1, tl.y() + 2 * radius + 1 };

    for ( int y = tl.y(); y < br.y(); y++ )
    {
        for ( int x = tl.x(); x < br.x(); x++ )
        {
            int xi = origin.x() - x;
            int yi = origin.y() - y;

            if ( xi * xi + yi * yi <= r2 )
            {
                gr.push_back({x, y});
            }
        }
    }

    return std::move(gr);
}

GridRegion GridUtils::createCone(Vector2i origin, int length, Direction direction)
{
    GridRegion out;
    if ( direction == Direction::N || direction == Direction::E
    || direction == Direction::S || direction == Direction::W )
    {

    }
    else
    {

        for ( int y = 0; y > -length; y-- )
        {
            for ( int x = 0; x < length; x++ )
            {
                if ( x - y < length )
                    out.push_back({x, y});
            }
        }
    }

    MatrixTransform::TransMat const* transform;

    switch ( direction )
    {

        case Direction::N:
            break;
        case Direction::NE:
            transform = &MatrixTransform::identity;
            break;
        case Direction::E:
            break;
        case Direction::SE:
            transform = &MatrixTransform::rot90;
            break;
        case Direction::S:
            break;
        case Direction::SW:
            transform = &MatrixTransform::rot180;
            break;
        case Direction::W:
            break;
        case Direction::NW:
            transform = &MatrixTransform::rot270;
            break;
    }


    std::transform(out.begin(), out.end(), out.begin(), [&](auto& elem) {
        return MatrixTransform::transform(transform, elem) + AllNeighbours[direction] + origin;
    });

    return out;
}


Vector2i GridUtils::mapToFirstOctant(Vector2i pos, int octant)
{
   auto& mat = MatrixTransform::nthToFirstOctant[octant];
   return Vector2i{ mat[0] * pos.x() + mat[1] * pos.y(), mat[2] * pos.x() + mat[3] * pos.y()  };
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

Vector2i MatrixTransform::transform(MatrixTransform::TransMat const* mat, Vector2i target)
{
    return Vector2i{ (*mat)[0] * target.x() + (*mat)[1] * target.y(),
                     (*mat)[2] * target.x() + (*mat)[3] * target.y()  };
}
