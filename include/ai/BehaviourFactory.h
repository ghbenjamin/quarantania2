#pragma once

#include <ai/BehaviourTree.h>
#include <ai/BehaviourNodes.h>

namespace BehaviourFactory
{

// Does nothing, always passes the turn
BehaviourTree inertBehaviour();

// Try to move to the nearest enemy, and then try to attack it.
BehaviourTree moveAndAttackNearestEnemy();

}