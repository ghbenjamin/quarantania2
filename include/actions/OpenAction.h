#pragma once

#include <game/Action.h>

class OpenAction : public Action
{
public:

    OpenAction(Level* level, EntityRef opener, EntityRef target);
    ~OpenAction() override = default;

    ActionResult perform() override;

private:

    EntityRef m_opener;
    EntityRef m_target;

};