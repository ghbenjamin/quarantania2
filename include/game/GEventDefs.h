#pragma once

#include <game/GEvent.h>
#include <game/Entity.h>

namespace GEvents
{

struct EntityMove : public GEvent<EntityMove>
{
    EntityMove(EntityRef ent, const Vector2i &oldPos, const Vector2i &newPos)
    : ent(ent), oldPos(oldPos), newPos(newPos)
    {}

    EntityRef ent;
    Vector2i oldPos;
    Vector2i newPos;
};


}
