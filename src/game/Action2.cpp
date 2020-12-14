#include <game/Action2.h>
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

MoveAction::MoveAction(Level* level, const std::string &id, int range)
    : Action(level, id), m_range(range) { }

GridRegion MoveAction::possibleTiles(EntityRef source) const
{

}
