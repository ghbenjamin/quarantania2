#include <actions/Action.h>

Action::Action(Level* level)
: m_level(level)
{ }

EntityAction::EntityAction(Level* level, EntityRef actor, EntityRef subject)
: Action(level), m_actor(actor), m_subject(subject)
{ }

void EntityAction::setActor(EntityRef actor)
{
    m_actor = actor;
}

void EntityAction::setSubject(EntityRef subject)
{
    m_subject = subject;
}

EntityAction::EntityAction(Level *level)
: Action(level), m_actor(EntityNull), m_subject(EntityNull)
{
}

TileAction::TileAction(Level* level, EntityRef actor, Vector2i tile)
: Action(level), m_actor(actor), m_tile(tile)
{ }