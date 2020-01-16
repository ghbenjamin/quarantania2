#pragma once

#include <actions/Action.h>

class StepAction : public TileAction
{
public:
    using TileAction::TileAction;

    const char* description() const override;
    bool canTryAction() const override;
    bool doAction() const override;
};


class OpenAction : public EntityAction
{
public:
    using EntityAction::EntityAction;

    const char* description() const override;
    bool canTryAction() const override;
    bool doAction() const override;
};


class CloseAction : public EntityAction
{
public:
    using EntityAction::EntityAction;

    const char* description() const override;
    bool canTryAction() const override;
    bool doAction() const override;
};


class UnlockAction : public EntityAction
{
public:
    using EntityAction::EntityAction;

    const char* description() const override;
    bool canTryAction() const override;
    bool doAction() const override;
};

class ExitLevelAction : public EntityAction
{
public:
    using EntityAction::EntityAction;

    const char* description() const override;
    bool canTryAction() const override;
    bool doAction() const override;
};

class MeleeAttackAction : public EntityAction
{
public:
    using EntityAction::EntityAction;

    const char* description() const override;
    bool canTryAction() const override;
    bool doAction() const override;
};

class PickUpItemAction : public EntityAction
{
public:
    using EntityAction::EntityAction;

    const char* description() const override;
    bool canTryAction() const override;
    bool doAction() const override;
};



