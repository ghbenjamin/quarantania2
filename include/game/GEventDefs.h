#pragma once

#include <game/GEvent.h>
#include <game/Entity.h>

namespace GEvents
{

struct EntityMove : public GEvent<EntityMove>
{
    EntityMove(EntityRef ent, const Vector2i &oldPos, const Vector2i &newPos);
    ~EntityMove() override = default;

    EntityRef ent;
    Vector2i oldPos;
    Vector2i newPos;
};

struct EntityReady : public GEvent<EntityReady>
{
    explicit EntityReady(EntityRef ent);
    ~EntityReady() override = default;

    EntityRef ent;
};

struct EntityOpenClose : public GEvent<EntityOpenClose>
{
    explicit EntityOpenClose(EntityRef ent, bool isOpen);
    ~EntityOpenClose() override = default;

    EntityRef ent;
    bool isOpen;
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
