#pragma once

#include <unordered_map>
#include <utils/Containers.h>
#include <utils/Matrix.h>

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

// Octants 1 to 8:
//
//   \77|88/
//   6\7|8/1
//   66\|/11
//  ---------
//   55/|\22
//   5/4|3\2
//   /44|33\
//


namespace MatrixTransform
{
    using TransMat = int[4];

    static constexpr TransMat identity = {1, 0, 0, 1};
    static constexpr TransMat rot270 = { 0, 1, -1, 0 };
    static constexpr TransMat rot180 = { -1, 0, 0, -1 };
    static constexpr TransMat rot90 = { 0, -1, 1, 0 };

    static constexpr int nthToFirstOctant [8][4] = {
        {1, 0, 0, 1},
        {0, 1, 1, 0},
        {0, -1, 1, 0},
        {-1, 0, 0, 1},
        {-1, 0, 0, -1},
        {0, -1, -1, 0},
        {0, 1, -1, 0},
        {1, 0, 0, -1}
    };

    static const Matrix2i octantTransforms [8] = {
        Matrix2i{1, 0, 0, 1},
        Matrix2i{0, 1, 1, 0},
        Matrix2i{0, -1, 1, 0},
        Matrix2i{-1, 0, 0, 1},
        Matrix2i{-1, 0, 0, -1},
        Matrix2i{0, -1, -1, 0},
        Matrix2i{0, 1, -1, 0},
        Matrix2i{1, 0, 0, -1}
    };

    Vector2i transform( TransMat const* mat, Vector2i target );
}

Direction operator | (Direction lhs, Direction rhs);
Direction& operator |= (Direction& lhs, Direction rhs);
GridBitmask operator & (GridBitmask lhs, Direction rhs);
GridBitmask operator | (GridBitmask lhs, Direction rhs);
GridBitmask& operator|= (GridBitmask& lhs, Direction rhs);
bool operator== (GridBitmask& lhs, Direction rhs);

using GridRegion = std::vector<Vector2i>;

class GridUtils
{
public:
    static const GridBitmask CardinalOnly;

    static std::unordered_map<Direction, Vector2i> AllNeighbours;
    static std::unordered_map<Direction, Vector2i> CardinalNeighbours;

    static bool isCardinal( Direction dir );

    static GridRegion createLine( Vector2i start, Vector2i end );
    static GridRegion createRect( Vector2i origin, Vector2i size );
    static GridRegion createCircle( Vector2i origin, int radius );
    static GridRegion createCone( Vector2i origin, int length, Direction direction );

    static Vector2i mapToFirstOctant( Vector2i pos, int octant );


    static bool isAdjacent(Vector2i lhs, Vector2i rhs);

private:

    static GridRegion createLineLow( int x0, int y0, int x1, int y1 );
    static GridRegion createLineHigh( int x0, int y0, int x1, int y1 );

};

