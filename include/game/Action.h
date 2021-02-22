#pragma once

#include <string>
#include <resource/Spritesheet.h>
#include <game/RawData.h>
#include <game/Grid.h>
#include <engine/Entity.h>
#include <utils/GridUtils.h>
#include <game/ResourceDatabase.h>

class Level;

enum class TargetingType
{
    SingleTile,
    GridRegion,
    SingleEntity,
};

class IActionTargeting
{
public:
    IActionTargeting(Level* level, EntityRef actor);
    virtual ~IActionTargeting() = default;

protected:
    Level* m_level;
    EntityRef m_actor;
};

struct GameAction
{
    template <typename T>
    GameAction(std::string const& id, TargetingType ttype, const std::shared_ptr<T> &impl)
    : data(ResourceDatabase::instance().actionFromId(id)),
      ttype(ttype),
      impl(std::static_pointer_cast<IActionTargeting>(impl))
      {}

    ActionData data;
    TargetingType ttype;
    std::shared_ptr<IActionTargeting> impl;
};





class SingleTileTargeting : public IActionTargeting
{
public:
    SingleTileTargeting(Level* level, EntityRef actor);
    ~SingleTileTargeting() override = default;

    virtual void perform( Vector2i tile ) = 0;

    virtual bool isMovement() const = 0;
    virtual GridRegion getValidTiles() = 0;
    virtual bool tileIsValid(Vector2i tile) = 0;

    virtual std::vector<Vector2i> pathToTile(Vector2i tile);
};


class SingleEntityTargeting : public IActionTargeting
{
public:
    SingleEntityTargeting(Level* level, EntityRef actor);
    ~SingleEntityTargeting() override = default;

    virtual void perform( EntityRef target ) = 0;
    virtual bool entityIsValid(EntityRef ref) = 0;

};


class ActionMoveParent : public SingleTileTargeting
{
public:
    ActionMoveParent(Level* level, EntityRef actor, int range);
    ~ActionMoveParent() override = default;

    bool isMovement() const override;
    GridRegion getValidTiles() override;
    bool tileIsValid(Vector2i tile) override;
    std::vector<Vector2i> pathToTile(Vector2i tile) override;

protected:
    int m_range;
    PathMap m_pathMap;
};



