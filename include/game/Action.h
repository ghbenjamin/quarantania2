#pragma once

#include <functional>
#include <utils/Containers.h>
#include <engine/Entity.h>

// Forward definitions
class Level;
class Action;

// Typedefs
using ActionPtr = std::shared_ptr<Action>;


class Action
{
public:
    Action(Level* level);
    virtual ~Action() = default;

    // *short* description of the action, e.g. 'Open'
    virtual const char* description() const = 0;

    // Can this action be attempted? e.g. is this a sensible thing to attempt to do?
    virtual bool canTryAction() const = 0;

    // Attempt to perform the action, returning whether or not we succeeded.
    virtual bool doAction() const = 0;

protected:
    Level* m_level;
};


class EntityAction : public Action
{
public:
    EntityAction(Level* level, EntityRef actor, EntityRef subject);
    EntityAction(Level* level);
    ~EntityAction() override = default;

    void setActor(EntityRef actor);
    void setSubject(EntityRef subject);

protected:
    EntityRef m_actor;
    EntityRef m_subject;
};


class TileAction : public Action
{
public:
    TileAction(Level* level, EntityRef actor, Vector2i tile);
    ~TileAction() override = default;

protected:
    EntityRef m_actor;
    Vector2i m_tile;
};