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

struct LevelReady : public GEvent<LevelReady>
{
    LevelReady() = default;
    ~LevelReady() override = default;
};

struct GameTick : public GEvent<GameTick>
{
    GameTick() = default;
    ~GameTick() override = default;
};

}
