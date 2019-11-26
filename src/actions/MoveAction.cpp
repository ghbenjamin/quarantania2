#include <actions/MoveAction.h>
#include <game/Level.h>
#include <components/TilePosition.h>

MoveAction::MoveAction(Level *level, Vector2i tile)
        : TileAction(level, tile)
{
}

bool MoveAction::doAction(EntityRef actor) const
{
    auto tilePos = m_level->getComponents<Components::TilePosition>(actor);

    if ( m_level->grid().pass().valueAt(m_tile) != Rules::Passibility::Impassable )
    {
        m_level->events().broadcast<GEvents::EntityMove>( actor, tilePos->position, m_tile );
        return true;
    }

    return false;
}


