#pragma once

#include <string>
#include <variant>

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

using ActionTarget = std::variant<
        Vector2i,       // Single Tile
        GridRegion,     // Grid Region
        EntityRef       // Single entity
    >;


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
      impl(std::static_pointer_cast<IActionTargeting>(impl)),
      enabled(true)
      {}

    ActionData data;
    TargetingType ttype;
    std::shared_ptr<IActionTargeting> impl;
    bool enabled;
};


struct ReifiedGameAction
{
    GameAction action;
    ActionTarget target;

    ReifiedGameAction(const GameAction &action, const ActionTarget &target);
    void perform( Level* level, EntityRef actor );
};

struct ActionSpeedData
{
    ActionSpeedData( GameAction const* action );
    ActionSpeedData( GameAction* action );
    
    GameAction const* action = nullptr;
    ActionSpeed modified = ActionSpeed::Free;
};


class ActionsUsedInfo
{
public:
    ActionsUsedInfo() = default;
    ~ActionsUsedInfo() = default;

    // Can this actor perform the specified action? This will depend on the actions the actor has already
    // taken this round.
    bool canUseAction( ActionSpeed speed ) const;

    // Return a three-member array of bools describing whether or not the actor has used his move, standard, and
    // swift actions this turn.
    std::array<bool, 3> getUsedActions() const;


    // Mark that the actor has used the specified action.
    void useAction( ActionSpeed speed );

    // Reset the actions used info, re-enabling all actions.
    void reset();

private:
    bool m_usedStandard = false;
    bool m_usedMove = false;
    bool m_usedSwift = false;
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
    virtual GridRegion getValidTiles();
};

