#pragma once

#include <game/Action.h>

// Abstract types
// ---------------------------


class ActionMoveParent : public SingleTileTargeting
{
public:
    ActionMoveParent();
    ActionMoveParent(int maxRange);
    ~ActionMoveParent() override = default;

    void attachImpl() override;

    bool isMovement() const override;
    GridRegion getValidTiles() override;
    bool tileIsValid(Vector2i tile) override;
    std::vector<Vector2i> pathToTile(Vector2i tile) override;

protected:
    int m_range;
    int m_maxRange;

    PathMap m_pathMap;
};


class ActionAttackParent : public SingleEntityTargeting
{
public:
    bool entityIsValid(EntityRef ref) override;
    void attachImpl() override;

protected:
    float m_reach = -1;
};


// Concrete types
// ------------------------

class ActionMoveStride : public ActionMoveParent
{
public:
    using ActionMoveParent::ActionMoveParent;
    void perform(Vector2i tile) override;
};


class ActionMoveStep : public ActionMoveParent
{
public:
    ActionMoveStep();
    void perform(Vector2i tile) override;
};


class ActionMeleeAttack : public ActionAttackParent
{
public:
    using ActionAttackParent::ActionAttackParent;
    void perform(EntityRef target) override;
};


class ActionPowerAttack : public ActionAttackParent
{
public:
    using ActionAttackParent::ActionAttackParent;
    void perform(EntityRef target) override;
};



