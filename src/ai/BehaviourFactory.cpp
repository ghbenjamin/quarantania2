#include <ai/BehaviourFactory.h>
#include <ai/EntityTargetingScheme.h>
#include <utils/Memory.h>


BehaviourTree BehaviourFactory::inertBehaviour()
{
    return BehaviourTree();
}

BehaviourTree BehaviourFactory::moveAndAttackNearestEnemy()
{
    auto targeting = std::make_shared<EntityTargetingNearest>();
    auto root = std::make_shared<BehaviourNodes::MoveOrPerformAction>(
        targeting,
        Utils::make_shared_with_type<BTNode, BehaviourNodes::MoveToTarget>(targeting),
        Utils::make_shared_with_type<BTNode, BehaviourNodes::MeleeAttackTarget>(targeting)
    );
    
    return BehaviourTree(root);
}
