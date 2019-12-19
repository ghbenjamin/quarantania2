#include <actions/ActionDefs.h>
#include <game/Level.h>
#include <components/All.h>
#include <utils/GridUtils.h>

bool StepAction::doAction(EntityRef actor) const
{
    auto tilePos = m_level->getComponents<Components::TilePosition>(actor);
    m_level->events().broadcast<GEvents::EntityMove>( actor, tilePos->position, m_tile );

    return true;
}

bool StepAction::canTryAction(EntityRef actor) const
{
    auto tilePos = m_level->getComponents<Components::TilePosition>(actor);

    if ( !GridUtils::isAdjacent(tilePos->position, m_tile ) )
    {
        return false;
    }

    else if ( m_level->grid().pass().valueAt(m_tile) == Rules::Passibility::Impassable )
    {
        return false;
    }

    return true;
}

const char *StepAction::description() const
{
    return "Step";
}


bool OpenAction::doAction(EntityRef actor) const
{
    m_level->events().broadcast<GEvents::EntityOpenClose>( m_entity, true );
    return true;
}

bool OpenAction::canTryAction(EntityRef actor) const
{
    auto openable = m_level->getComponents<Components::Openable>( m_entity );
    return openable->isOpen;
}

const char *OpenAction::description() const
{
    return "Open";
}

bool CloseAction::doAction(EntityRef actor) const
{
    m_level->events().broadcast<GEvents::EntityOpenClose>( m_entity, false );
    return true;
}

bool CloseAction::canTryAction(EntityRef actor) const
{
    return false;
}

const char *CloseAction::description() const
{
    return "Close";
}