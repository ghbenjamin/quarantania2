#include <game/ActionDefs.h>

#include <game/Level.h>
#include <components/PositionComponent.h>
#include <components/ActorComponent.h>
#include <game/AttackDefs.h>


ActionMoveParent::ActionMoveParent(int maxRange)
        : m_maxRange(maxRange), m_range(-1) {}

ActionMoveParent::ActionMoveParent()
        : m_range(-1), m_maxRange(-1) {}

bool ActionMoveParent::isMovement() const
{
    return true;
}

GridRegion ActionMoveParent::getValidTiles()
{
    GridRegion gr;
    for (auto const&[k, v] : m_pathMap )
    {
        gr.push_back(k);
    }

    return std::move(gr);
}

bool ActionMoveParent::tileIsValid(Vector2i tile)
{
    return m_pathMap.find(tile) != m_pathMap.end();
}

std::vector<Vector2i> ActionMoveParent::pathToTile(Vector2i tile)
{
    return m_level->grid().pathFromPathMap(m_pathMap, tile);
}

void ActionMoveParent::attachImpl()
{
    auto posC = m_level->ecs().getComponents<PositionComponent>(m_actor);
    auto actorC = m_level->ecs().getComponents<ActorComponent>(m_actor);

    m_range = actorC->actor.getSpeed();

    if (m_maxRange >= 0)
    {
        m_range = std::min(m_range, m_maxRange);
    }

    m_pathMap = m_level->grid().allPathsFromTile(posC->tilePosition, m_range);
}


bool ActionAttackParent::entityIsValid(EntityRef ref)
{
    auto actorC = m_level->ecs().tryGetComponent<ActorComponent>(ref);
    if ( actorC )
    {
        auto posDef = m_level->ecs().tryGetComponent<PositionComponent>(ref);
        auto posAtk = m_level->ecs().tryGetComponent<PositionComponent>(m_actor);

        if ( m_level->entityDistance( m_actor, ref ) <= m_reach )
        {
            return true;
        }
    }

    return false;
}

void ActionAttackParent::attachImpl()
{
    auto actorC = m_level->ecs().tryGetComponent<ActorComponent>(m_actor);
    m_reach = actorC->actor.getReach();
}



void ActionMoveStride::perform(Vector2i tile)
{
    auto origin = m_level->ecs().getComponents<PositionComponent>(m_actor)->tilePosition;
    auto pathInfo = m_level->grid().pathBetweenPoints( origin, tile );

    std::vector<Vector2i> path;
    for ( auto const& item : pathInfo )
    {
        path.push_back(item.first);
    }

    m_level->events().broadcast<GameEvents::EntityMove>(m_actor, origin, tile, path);
}

ActionMoveStep::ActionMoveStep()
        : ActionMoveParent(1) {}


void ActionMoveStep::perform(Vector2i tile)
{
    auto origin = m_level->ecs().getComponents<PositionComponent>(m_actor)->tilePosition;
    std::vector<Vector2i> path = { tile };

    m_level->events().broadcast<GameEvents::EntityMove>(m_actor, origin, tile, path);
}


void ActionMeleeAttack::perform(EntityRef target)
{
    m_level->events().broadcast<GameEvents::CombatMeleeAttack>(
        m_actor, target, std::make_shared<MeleeAttackStandard>()
    );
}

void ActionPowerAttack::perform(EntityRef target)
{
    m_level->events().broadcast<GameEvents::CombatMeleeAttack>(
        m_actor, target, std::make_shared<MeleeAttackPowerAttack>()
    );
}

void ActionFullAttack::perform( EntityRef target )
{
    m_level->events().broadcast<GameEvents::CombatMeleeAttack>(
        m_actor, target, std::make_shared<MeleeAttackFullAttack>()
    );
}
