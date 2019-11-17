#pragma once

#include <vector>
#include <string>
#include <memory>


#include <game/Entity.h>

class Level;
struct ActionResult;
class Action;

using ActionPtr = std::unique_ptr<Action>;

struct ActionResult
{
    bool succeeded;
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