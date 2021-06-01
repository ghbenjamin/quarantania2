#include <ai/BehaviourNodes.h>
#include <game/Level.h>
#include <components/PositionComponent.h>
#include <components/ActorComponent.h>
#include <game/ActionDefs.h>

std::shared_ptr<ReifiedGameAction> BehaviourNodes::MeleeAttackNearestTarget::evaluate(Level *level, EntityRef actor)
{

    auto const& [posC, actorC] = level->ecs().getComponents<PositionComponent, ActorComponent>(actor);

    auto attackTargeting = std::make_shared<ActionMeleeAttack>();
    attackTargeting->attach(level, actor);

    std::unordered_map<EntityRef, int> distances;

    for ( auto ref : level->ecs().entitiesHaving<ActorComponent>() )
    {
        auto const& [pcC, pcPosC] = level->ecs().getComponents<ActorComponent, PositionComponent>(ref);
        if (pcC->actorType == ActorType::PC)
        {
            int dx = posC->tilePosition.x() - pcPosC->tilePosition.x();
            int dy = posC->tilePosition.y() - pcPosC->tilePosition.y();
            distances[ref] = dx * dx + dy * dy;
        }
    }

    auto closest = std::min_element(distances.begin(), distances.end(),
            [](auto const& p1, auto const& p2) { return p1.second < p2.second; });

    EntityRef target = closest->first;

    // If we can't attack the closest entity, give up
    // TODO: Do something better here?
    if (!attackTargeting->entityIsValid(target))
    {
        return {};
    }

    GameAction gameAction = { "strike", TargetingType::SingleEntity, attackTargeting };

    ActionSpeedData asd;
    asd.action = &gameAction;
    actorC->actor.applyAllModifiers( &asd );

    // If we don't have enough actions left to perform this action, wait till next turn
    if ( !actorC->actor.actionInfo().canUseAction( asd.modified ))
    {
        return {};
    }

    return std::make_shared<ReifiedGameAction>(gameAction, target);
}

std::shared_ptr<ReifiedGameAction> BehaviourNodes::Identity::evaluate(Level *level, EntityRef actor)
{
    if ( m_children.empty() )
    {
        return {};
    }

    return m_children.front()->evaluate(level, actor);
}
