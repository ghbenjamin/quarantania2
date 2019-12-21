#include <actions/ActionDefs.h>
#include <game/Level.h>
#include <components/All.h>
#include <utils/GridUtils.h>

bool StepAction::doAction() const
{
    auto tilePos = m_level->getComponents<Components::TilePosition>(m_actor);
    m_level->events().broadcast<GEvents::EntityMove>( m_actor, tilePos->position, m_tile );

    return true;
}

bool StepAction::canTryAction() const
{
    auto tilePos = m_level->getComponents<Components::TilePosition>(m_actor);

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
    return "Move";
}


bool OpenAction::doAction() const
{
    m_level->events().broadcast<GEvents::EntityOpenClose>( m_entity, true );
    return true;
}

bool OpenAction::canTryAction() const
{
    auto openable = m_level->getComponents<Components::Openable>( m_entity );
    if (!openable) return false;

    return !openable->isOpen;
}

const char *OpenAction::description() const
{
    return "Open";
}

bool CloseAction::doAction() const
{
    m_level->events().broadcast<GEvents::EntityOpenClose>( m_entity, false );
    return true;
}

bool CloseAction::canTryAction() const
{
    auto openable = m_level->getComponents<Components::Openable>( m_entity );
    if (!openable) return false;

    return openable->isOpen;
}

const char *CloseAction::description() const
{
    return "Close";
}

const char *UnlockAction::description() const
{
    return "Unlock";
}

bool UnlockAction::canTryAction() const
{
    auto lockable = m_level->getComponents<Components::Lockable>( m_entity );
    if (!lockable) return false;
    return lockable->isLocked;
}

bool UnlockAction::doAction() const
{
    auto lockable = m_level->getComponents<Components::Lockable>( m_entity );
    return false;
}