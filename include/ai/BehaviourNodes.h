#pragma once

#include <ai/BehaviourTree.h>


namespace BehaviourNodes
{

//
class Identity : public BTNode
{
public:
    std::shared_ptr<ReifiedGameAction> evaluate(Level *level, EntityRef actor) override;
};

class MeleeAttackNearestTarget : public BTNode
{
public:
    std::shared_ptr<ReifiedGameAction> evaluate(Level* level, EntityRef actor) override;
};

}