#pragma once 

#include <vector>
#include <memory>
#include <game/Action.h>

struct GameAction;
class Level;

class BTNode
{
public:
    
    BTNode() = default;
    virtual ~BTNode() = default;
    
    virtual std::shared_ptr<ReifiedGameAction> evaluate(Level* level, EntityRef actor) = 0;
    
    template <typename T, typename...Args>
    void addChild( Args&&... args )
    {
        static_assert(std::is_base_of_v<BTNode, T>);
        auto ptr = std::make_shared<T>( std::forward<Args>(args)... );
        m_children.push_back( std::static_pointer_cast<BTNode>(ptr) );
    }

    template <typename T>
    void addChild( std::shared_ptr<T> ptr )
    {
        static_assert(std::is_base_of_v<BTNode, T>);
        m_children.push_back( std::static_pointer_cast<BTNode>(ptr) );
    }
    
    int childCount() const;
    std::vector<std::shared_ptr<BTNode>> const& children() const;


protected:
    std::vector<std::shared_ptr<BTNode>> m_children; 
};



class BehaviourTree
{
public:
    
    template <typename T>
    BehaviourTree( std::shared_ptr<T> root )
        : m_root( std::static_pointer_cast<BTNode>( root )) {}
        
    BehaviourTree();
    ~BehaviourTree() = default;

    std::shared_ptr<ReifiedGameAction> evaluate(Level* level, EntityRef actor);
    std::shared_ptr<BTNode>& root();

private:
    
    std::shared_ptr<BTNode> m_root; 
};
