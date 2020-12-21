#include <game/Action.h>
#include <game/ResourceDatabase.h>

Action::Action(Level* level, std::string const& id)
        : m_data( ResourceDatabase::instance().actionFromId(id) ),
          m_level(level) {}


std::string const &Action::getName() const
{
    return m_data.name;
}

std::string const &Action::getDescription() const
{
    return m_data.description;
}

SpritesheetKey const &Action::getSprite() const
{
    return m_data.sprite;
}

bool Action::getProvokes() const
{
    return m_data.provokes;
}

void Action::setEnabled(bool val)
{
    m_enabled = val;
}

bool Action::isEnabled() const
{
    return m_enabled;
}

RawActionDataType Action::getType() const
{
    return m_data.type;
}

MoveAction::MoveAction(Level* level, const std::string &id, int range)
        : Action(level, id), m_range(range) { }

AttackAction::AttackAction(Level *level, const std::string &id)
        : Action(level, id) { }
