#include <game/ActionDefs.h>

#include <game/Level.h>
#include <components/PositionComponent.h>
#include <components/ActorComponent.h>

ActionMoveStride::ActionMoveStride(Level * level, EntityRef actor, int range)
        : ActionMoveParent(level, actor, range) {}

void ActionMoveStride::perform(Vector2i tile)
{
    auto origin = m_level->ecs().getComponents<PositionComponent>(m_actor)->tilePosition;
    auto pathMap = m_level->grid().pathFromPathMap(m_pathMap, tile);

    m_level->events().broadcast<GameEvents::EntityMove>(m_actor, origin, tile, pathMap);
    m_level->events().broadcast<GameEvents::EntityAction>(m_actor, m_range);
}

ActionMoveStep::ActionMoveStep(Level* level, EntityRef actor)
        : ActionMoveParent(level, actor, 1) {}


void ActionMoveStep::perform(Vector2i tile)
{
    auto origin = m_level->ecs().getComponents<PositionComponent>(m_actor)->tilePosition;
    auto pathMap = m_level->grid().pathFromPathMap(m_pathMap, tile);

    m_level->events().broadcast<GameEvents::EntityMove>(m_actor, origin, tile, pathMap);
    m_level->events().broadcast<GameEvents::EntityAction>(m_actor, m_range);
}


ActionMeleeAttack::ActionMeleeAttack(Level *level, EntityRef actor, float reach)
        : SingleEntityTargeting(level, actor), m_reach(reach)  {}

void ActionMeleeAttack::perform(EntityRef target)
{
    m_level->events().broadcast<GameEvents::CombatMeleeAttack>(m_actor, target);
}

bool ActionMeleeAttack::entityIsValid(EntityRef ref)
{
    auto actorC = m_level->ecs().tryGetComponent<ActorComponent>(ref);
    if ( actorC && (actorC->actorType == ActorType::NPC) )
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



ActionPowerAttack::ActionPowerAttack(Level *level, EntityRef actor, float reach)
        : SingleEntityTargeting(level, actor), m_reach(reach)  {}

void ActionPowerAttack::perform(EntityRef target)
{
    // Do the thing!
}

bool ActionPowerAttack::entityIsValid(EntityRef ref)
{
    auto actorC = m_level->ecs().tryGetComponent<ActorComponent>(ref);
    return ( actorC && (actorC->actorType == ActorType::NPC ) );
}

