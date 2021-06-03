#include <ai/BehaviourTree.h>
#include <ai/BehaviourNodes.h>

int BTNode::childCount() const
{
    return (int) m_children.size();
}

std::vector<std::shared_ptr<BTNode>> const &BTNode::children() const
{
    return m_children;
}

std::shared_ptr<ReifiedGameAction> BehaviourTree::evaluate(Level* level, EntityRef actor)
{
    if (!m_root)
    {
        return {};
    }

    auto action = m_root->evaluate(level, actor);
    
    
    
    return action;
}

std::shared_ptr<BTNode> &BehaviourTree::root()
{
    return m_root;
}

BehaviourTree::BehaviourTree()
{
    m_root = std::static_pointer_cast<BTNode>(
            std::make_shared<BehaviourNodes::Identity>() );
}