#include <game/LevelData.h>

using namespace LD;

Vector2i Room::centre() const
{
    return Vector2i {
        bounds.x() + ( bounds.w() / 2 ),
        bounds.y() + ( bounds.h() / 2 )
    };
}