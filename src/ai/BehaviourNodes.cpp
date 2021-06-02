#include <ai/BehaviourNodes.h>
#include <ai/EntityTargetingScheme.h>
#include <game/Level.h>
#include <components/PositionComponent.h>
#include <components/ActorComponent.h>
#include <game/ActionDefs.h>

BehaviourNodes::MeleeAttackTarget::MeleeAttackTarget( std::shared_ptr<EntityTargetingScheme> targeting )
        : m_targeting(targeting) {}

std::shared_ptr<ReifiedGameAction> BehaviourNodes::MeleeAttackTarget::evaluate( Level *level, EntityRef actor)
{
    auto const& [posC, actorC] = level->ecs().getComponents<PositionComponent, ActorComponent>(actor);
    
    auto targetOpt = m_targeting->getEntity(level, actor);
    if (!targetOpt)
    {
        return {};
    }
    
    EntityRef target = *targetOpt;
    
    auto attackTargeting = std::make_shared<ActionMeleeAttack>();
    attackTargeting->attach(level, actor);


    // If we can't attack the closest entity, give up
    if (!attackTargeting->entityIsValid(target))
    {
        return {};
    }

    GameAction gameAction = { "strike", TargetingType::SingleEntity, attackTargeting };
    
    // If we don't have enough actions left to perform this action, wait till next turn
    if ( !actorC->actor.canPerformAction( gameAction ))
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


BehaviourNodes::MoveToTarget::MoveToTarget( std::shared_ptr<EntityTargetingScheme> targeting )
        : m_targeting(targeting) {}

std::shared_ptr<ReifiedGameAction> BehaviourNodes::MoveToTarget::evaluate( Level *level, EntityRef actor )
{
    auto const& [posC, actorC] = level->ecs().getComponents<PositionComponent, ActorComponent>(actor);
    
    auto targetOpt = m_targeting->getEntity(level, actor);
    if (!targetOpt)
    {
        return {};
    }
    
    EntityRef target = *targetOpt;
    
    auto targetPosC = level->ecs().getComponents<PositionComponent>(target);
    
    
    
    
    auto moveTargeting = std::make_shared<ActionMoveStride>();
    moveTargeting->attach(level, actor);
    
    GameAction gameAction = { "move", TargetingType::SingleTile, moveTargeting };
    
    // If we don't have enough actions left to perform this action, wait till next turn
    if ( !actorC->actor.canPerformAction( gameAction ))
    {
        return {};
    }
}

