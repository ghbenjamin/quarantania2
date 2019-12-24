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
    m_level->events().broadcast<GEvents::EntityOpenClose>(m_subject, true );
    return true;
}

bool OpenAction::canTryAction() const
{
    auto openable = m_level->getComponents<Components::Openable>(m_subject );
    if (!openable) return false;

    if (openable->isOpen) return false;

    if (m_level->squaredEntityDistance( m_actor, m_subject ) > 2) return false;

    return true;
}

const char *OpenAction::description() const
{
    return "Open";
}

bool CloseAction::doAction() const
{
    m_level->events().broadcast<GEvents::EntityOpenClose>(m_subject, false );
    return true;
}

bool CloseAction::canTryAction() const
{
    auto openable = m_level->getComponents<Components::Openable>(m_subject );
    if (!openable) return false;

    if (!openable->isOpen) return false;

    if (m_level->squaredEntityDistance( m_actor, m_subject ) > 2) return false;

    return true;
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
    auto lockable = m_level->getComponents<Components::Lockable>(m_subject);
    if (!lockable) return false;
    return lockable->isLocked;
}

bool UnlockAction::doAction() const
{
    auto lockable = m_level->getComponents<Components::Lockable>(m_subject);
    return false;
}

const char *ExitLevelAction::description() const
{
    return "Descend";
}

bool ExitLevelAction::canTryAction() const
{
    if (m_level->squaredEntityDistance( m_actor, m_subject ) > 2) return false;

    return true;
}

bool ExitLevelAction::doAction() const
{
    Logging::log("WE MADE IT WOOOO");
    return true;
}
