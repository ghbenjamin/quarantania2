#include <actions/Action.h>

Action::Action(Level* level, EntityRef actor)
: m_level(level), m_actor(actor)
{ }

EntityAction::EntityAction(Level* level, EntityRef actor, EntityRef entity)
: Action(level, actor), m_entity(entity)
{ }

TileAction::TileAction(Level* level, EntityRef actor, Vector2i tile)
: Action(level, actor), m_tile(tile)
{ }
