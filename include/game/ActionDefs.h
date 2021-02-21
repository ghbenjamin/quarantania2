#pragma once

#include <game/Action.h>


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
    ActionMeleeAttack(Level* level, EntityRef actor, float reach);
    ~ActionMeleeAttack() override = default;

    void perform(EntityRef target) override;
    bool entityIsValid(EntityRef ref) override;

private:
    float m_reach;
};


class ActionPowerAttack : public SingleEntityTargeting
{
public:
    ActionPowerAttack(Level* level, EntityRef actor, float reach);
    ~ActionPowerAttack() override = default;

    void perform(EntityRef target) override;
    bool entityIsValid(EntityRef ref) override;

private:
    float m_reach;
};



