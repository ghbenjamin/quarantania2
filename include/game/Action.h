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
    IActionTargeting();
    virtual ~IActionTargeting() = default;

    void attach( Level* level, EntityRef actor );
    virtual void attachImpl();

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
    using IActionTargeting::IActionTargeting;

    virtual void perform( Vector2i tile ) = 0;
    virtual bool isMovement() const = 0;
    virtual GridRegion getValidTiles() = 0;
    virtual bool tileIsValid(Vector2i tile) = 0;
    virtual std::vector<Vector2i> pathToTile(Vector2i tile);
};


class SingleEntityTargeting : public IActionTargeting
{
public:
    using IActionTargeting::IActionTargeting;

    virtual void perform( EntityRef target ) = 0;
    virtual bool entityIsValid(EntityRef ref) = 0;
};

