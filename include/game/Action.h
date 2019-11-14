#pragma once

#include <vector>
#include <string>
#include <memory>


#include <game/Entity.h>

class Level;
struct ActionResult;
class Action;
struct ActionFuture;

using ActionPtr = std::unique_ptr<Action>;
using ActionFutureList = std::vector<ActionFuture>;

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


struct ActionFuture
{
    ActionFuture() = default;
    ~ActionFuture() = default;

    ActionFuture( ActionFuture const& other ) = delete;
    ActionFuture& operator=(ActionFuture const& other) = delete;

    ActionFuture( ActionFuture&& other ) = default;
    ActionFuture& operator=(ActionFuture&& other) = default;


    EntityRef actor = EntityNull;
    ActionPtr action;
    std::string name;
    std::string description;
};

