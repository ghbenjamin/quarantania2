#include <actions/Action.h>

Action::Action(Level* level)
: m_level(level)
{ }

ActionProcedurePtr Action::generate(EntityRef actor)
{
    return std::make_unique<ActionProcedure>([this, actor](){
        if (!canTryAction(actor))
        {
            return false;
        }
        else
        {
            return this->doAction(actor);
        }
    });
}

EntityAction::EntityAction(Level* level, EntityRef entity)
: Action(level), m_entity(entity)
{ }

TileAction::TileAction(Level* level, Vector2i tile)
: Action(level), m_tile(tile)
{ }
