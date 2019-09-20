#pragma once

#include <game/Entity.h>

class Level;

struct ActionResult
{
};

class Action
{
public:
    Action(Level* level);
    virtual ~Action() = default;

    virtual ActionResult perform() = 0;

protected:
    Level* m_level;
};


