#include <actions/MoveAction.h>

MoveAction::MoveAction(Level *level, EntityRef entity, Vector2i oldPos, Vector2i newPos)
: Action(level), m_entity(entity), m_oldPos(oldPos), m_newPos(newPos)
{
}
