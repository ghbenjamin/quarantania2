#pragma once

#include <actions/Action.h>

class StepAction : public TileAction
{
public:
    using TileAction::TileAction;

    const char* description() const override;
    bool canTryAction(EntityRef actor) const override;
    bool doAction(EntityRef actor) const override;
};


class OpenAction : public EntityAction
{
public:
    using EntityAction::EntityAction;

    const char* description() const override;
    bool canTryAction(EntityRef actor) const override;
    bool doAction(EntityRef actor) const override;
};


class CloseAction : public EntityAction
{
public:
    using EntityAction::EntityAction;

    const char* description() const override;
    bool canTryAction(EntityRef actor) const override;
    bool doAction(EntityRef actor) const override;
};