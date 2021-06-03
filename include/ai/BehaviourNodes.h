#pragma once

#include <ai/BehaviourTree.h>

class EntityTargetingScheme;

namespace BehaviourNodes
{

class Identity : public BTNode
{
public:
    std::shared_ptr<ReifiedGameAction> evaluate(Level *level, EntityRef actor) override;
};


class MeleeAttackTarget : public BTNode
{
public:
    MeleeAttackTarget( std::shared_ptr<EntityTargetingScheme> targeting );
    std::shared_ptr<ReifiedGameAction> evaluate(Level* level, EntityRef actor) override;

private:
    std::shared_ptr<EntityTargetingScheme> m_targeting;
};


class MoveToTarget : public BTNode
{
public:
    MoveToTarget( std::shared_ptr<EntityTargetingScheme> targeting );
    std::shared_ptr<ReifiedGameAction> evaluate(Level* level, EntityRef actor) override;
    
private:
    std::shared_ptr<EntityTargetingScheme> m_targeting;
};

class MoveOrPerformAction : public BTNode
{
public:
    MoveOrPerformAction( std::shared_ptr<EntityTargetingScheme> targeting, std::shared_ptr<BTNode> ifNotAdj, std::shared_ptr<BTNode> ifAdj );
    std::shared_ptr<ReifiedGameAction> evaluate(Level* level, EntityRef actor) override;

private:
    std::shared_ptr<EntityTargetingScheme> m_targeting;
};



}