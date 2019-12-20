#pragma once

#include <functional>
#include <utils/Containers.h>
#include <game/Entity.h>

// Forward definitions
class Level;
class Action;

// Typedefs
using ActionPtr = std::shared_ptr<Action>;


class Action
{
public:
    Action(Level* level, EntityRef actor);
    virtual ~Action() = default;

    // *short* description of the action, e.g. 'Open'
    virtual const char* description() const = 0;

    // Can this action be attempted? e.g. is this a sensible thing to attempt to do?
    virtual bool canTryAction() const = 0;

    // Attempt to perform the action, returning whether or not we succeeded.
    virtual bool doAction() const = 0;

protected:
    Level* m_level;
    EntityRef m_actor;
};


class EntityAction : public Action
{
public:
    EntityAction(Level* level, EntityRef actor, EntityRef entity);
    ~EntityAction() override = default;

protected:
    EntityRef m_entity;
};


class TileAction : public Action
{
public:
    TileAction(Level* level, EntityRef actor, Vector2i tile);
    ~TileAction() override = default;

protected:
    Vector2i m_tile;
};
