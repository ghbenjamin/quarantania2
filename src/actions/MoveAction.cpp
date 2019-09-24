#include <actions/MoveAction.h>
#include <game/Level.h>

MoveAction::MoveAction(Level *level, EntityRef entity, Vector2i oldPos, Vector2i newPos)
: Action(level), m_entity(entity), m_oldPos(oldPos), m_newPos(newPos)
{
}

ActionResult MoveAction::perform()
{
    if ( m_level->grid().pass().valueAt(m_newPos) != Rules::Passibility::Impassable )
    {
        m_level->events().broadcast<GEvents::EntityMove>( m_entity, m_oldPos, m_newPos );
        return { true };
    }

    return { false };
}
