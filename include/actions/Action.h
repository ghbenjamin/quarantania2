#pragma once

#include <functional>
#include <utils/Containers.h>
#include <game/Entity.h>

// Forward definitions
class Level;
class Action;

// Typedefs
using ActionProcedure = std::function<bool()>;
using ActionProcedurePtr = std::unique_ptr<ActionProcedure>;
using ActionPtr = std::shared_ptr<Action>;


class Action
{
public:
    Action(Level* level);
    virtual ~Action() = default;

    // *short* description of the action, e.g. 'Open'
    virtual const char* description() const = 0;
    virtual ActionProcedurePtr generate(EntityRef actor);

    // Can this action be attempted? e.g. is this a sensible thing to attempt to do?
    virtual bool canTryAction(EntityRef actor) const = 0;

    // Attempt to perform the action, returning whether or not we succeeded.
    virtual bool doAction(EntityRef actor) const = 0;

protected:
    Level* m_level;
};


class EntityAction : public Action
{
public:
    EntityAction(Level* level, EntityRef entity);
    ~EntityAction() override = default;

protected:
    EntityRef m_entity;
};


class TileAction : public Action
{
public:
    TileAction(Level* level, Vector2i tile);
    ~TileAction() override = default;

protected:
    Vector2i m_tile;
};
