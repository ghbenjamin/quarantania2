#pragma once

#include <actions/Action.h>
#include <utils/Containers.h>

class OpenAction : public EntityAction
{
public:
    OpenAction(Level* level, EntityRef entity);
    ~OpenAction() override = default;

protected:
    bool doAction(EntityRef actor) const override;
};