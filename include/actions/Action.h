#pragma once

#include <functional>
#include <utils/Containers.h>
#include <game/Entity.h>

using ActionProcedure = std::function<bool()>;
using ActionProcedurePtr = std::unique_ptr<ActionProcedure>;
class Level;


class Action
{
public:
    Action(Level* level);
    virtual ~Action() = default;

    virtual const std::string & description() const;
    virtual bool isVisible() const;
    virtual bool isEnabled() const;

    virtual ActionProcedurePtr generate(EntityRef actor);

protected:
    virtual bool doAction(EntityRef actor) const = 0;

protected:
    std::string m_description;
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
