#pragma once

#include <string>
#include <resource/Spritesheet.h>
#include <game/RawData.h>
#include <game/Grid.h>
#include <engine/Entity.h>
#include <utils/GridUtils.h>

class Level;

class Action
{
public:
    Action(Level* level, std::string const& id);
    virtual ~Action() = default;

    std::string const& getName() const;
    std::string const& getDescription() const;
    SpritesheetKey const& getSprite() const;
    bool getProvokes() const;
    bool isEnabled() const;
    RawActionDataType getType() const;

    void setEnabled(bool val);

protected:
    Level* m_level;

private:
    const RawActionData m_data;
    bool m_enabled;
};


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
    GameAction(const Action &data, const std::shared_ptr<IActionTargeting> &impl, TargetingType ttype);

    Action data;
    std::shared_ptr<IActionTargeting> impl;
    TargetingType ttype;

    template <typename T>
    static std::shared_ptr<GameAction> construct( Action const& data, std::shared_ptr<T> impl, TargetingType ttype )
    {
        static_assert( std::is_base_of_v<IActionTargeting, T> );

        return std::make_shared<GameAction>(
                data,
                std::static_pointer_cast<IActionTargeting>(impl),
                ttype
        );
    }
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




class ActionMoveStride : public ActionMoveParent
{
public:
    ActionMoveStride(Level* level, EntityRef actor, int range);
    ~ActionMoveStride() override = default;

    void perform(Vector2i tile) override;
};


class ActionMoveStep : public ActionMoveParent
{
public:
    ActionMoveStep(Level* level, EntityRef actor);
    ~ActionMoveStep() override = default;

    void perform(Vector2i tile) override;
};



class ActionMeleeAttack : public SingleEntityTargeting
{
public:
    using SingleEntityTargeting::SingleEntityTargeting;

    void perform(EntityRef target) override;
    bool entityIsValid(EntityRef ref) override;
};

class ActionPowerAttack : public SingleEntityTargeting
{
public:
    using SingleEntityTargeting::SingleEntityTargeting;

    void perform(EntityRef target) override;
    bool entityIsValid(EntityRef ref) override;
};
