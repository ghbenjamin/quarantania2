#pragma once

#include <actions/Action.h>
#include <utils/Containers.h>

class MoveAction : public Action
{
public:

    MoveAction(Level* level, EntityRef entity, Vector2i oldPos, Vector2i newPos );
    ~MoveAction() override = default;

private:

    EntityRef m_entity;
    Vector2i m_oldPos;
    Vector2i m_newPos;

};