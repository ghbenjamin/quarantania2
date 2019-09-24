#pragma once

#include <game/GEvent.h>
#include <game/Entity.h>

namespace GEvents
{

struct EntityMove : public GEvent<EntityMove>
{
    EntityMove(EntityRef ent, const Vector2i &oldPos, const Vector2i &newPos)
    : ent(ent), oldPos(oldPos), newPos(newPos) {}
    ~EntityMove() override = default;

    EntityRef ent;
    Vector2i oldPos;
    Vector2i newPos;
};

struct EntityReady : public GEvent<EntityReady>
{
    explicit EntityReady(EntityRef ent) : ent(ent) {}
    ~EntityReady() override = default;

    EntityRef ent;
};

struct FixedStateChange : public GEvent<FixedStateChange>
{
    FixedStateChange(EntityRef ent, int state) : ent(ent), state(state) {}
    ~FixedStateChange() override = default;

    EntityRef ent;
    int state;
};

struct GameTick : public GEvent<GameTick>
{
    GameTick() = default;
    ~GameTick() override = default;
};

}
