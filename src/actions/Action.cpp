#include <actions/Action.h>

bool Action::isVisible() const
{
    return true;
}

bool Action::isEnabled() const
{
    return true;
}

const std::string &Action::description() const
{
    return m_description;
}

Action::Action(Level* level)
: m_description("<default>"), m_level(level)
{
}

ActionProcedurePtr Action::generate(EntityRef actor)
{
    return std::make_unique<ActionProcedure>([this, actor](){
        return this->doAction(actor);
    });
}

EntityAction::EntityAction(Level* level, EntityRef entity)
: Action(level), m_entity(entity)
{ }

TileAction::TileAction(Level* level, Vector2i tile)
: Action(level), m_tile(tile)
{ }
