#pragma once

#include <actions/Action.h>
#include <utils/Containers.h>

class MoveAction : public TileAction
{
public:
    MoveAction(Level* level, Vector2i tile);
    ~MoveAction() override = default;

protected:
    bool doAction(EntityRef actor) const override;
};