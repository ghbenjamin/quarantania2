#include <game/Action.h>
#include <game/Level.h>
#include <components/PositionComponent.h>
#include <components/ActorComponent.h>


IActionTargeting::IActionTargeting()
        : m_level(nullptr), m_actor(EntityNull) {}

void IActionTargeting::attach(Level* level, EntityRef actor)
{
    m_level = level;
    m_actor = actor;

    attachImpl();
}

void IActionTargeting::attachImpl() {}

std::vector<Vector2i> SingleTileTargeting::pathToTile(Vector2i tile)
{
    return std::vector<Vector2i>();
}

bool ActionsUsedInfo::canUseAction( ActionSpeed speed ) const
{
    switch (speed)
    {
        case ActionSpeed::Standard:
            return !m_usedStandard;
        case ActionSpeed::Move:
            return !m_usedStandard || !m_usedMove;
        case ActionSpeed::FullRound:
            return !m_usedStandard && !m_usedMove;
        case ActionSpeed::Swift:
            return !m_usedSwift;
        case ActionSpeed::Free:
            return true;
    }
    
    return false;
}

void ActionsUsedInfo::useAction( ActionSpeed speed )
{
    AssertMsg( canUseAction(speed), "Invalid action selected" );

    switch ( speed )
    {
        case ActionSpeed::Standard:
            m_usedStandard = true;
            break;
        case ActionSpeed::Move:
            if (!m_usedMove)
            {
                m_usedMove = true;
            }
            else
            {
                m_usedStandard = true;
            }
            break;
        case ActionSpeed::FullRound:
            m_usedStandard = true;
            m_usedMove = true;
            break;
        case ActionSpeed::Swift:
            m_usedSwift = true;
            break;
        case ActionSpeed::Free:
            break;
    }
}

std::array<bool, 3> ActionsUsedInfo::getUsedActions() const
{
    return {
        m_usedStandard,
        m_usedMove,
        m_usedSwift
    };
}
