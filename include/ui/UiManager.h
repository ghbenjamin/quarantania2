#pragma once

#include <vector>

#include <ui/Element.h>

struct IEvent;
class InputInterface;
class RenderInterface;

namespace UI
{

class UiManager
{
public:

    UiManager() = default;
    ~UiManager() = default;

    bool input(IEvent &evt);
    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);

    template <typename WT, typename ...Args>
    std::shared_ptr<WT> createElement( Element* parent, Args...args )
    {
        auto ptr = std::make_shared<WT>( std::forward<Args>(args)... );
        if ( parent == nullptr )
        {
            m_roots.push_back(ptr);
        }
        else
        {
            parent->addChild(ptr);
        }

        ptr->m_manager = this;
        return ptr;
    }


private:

    std::vector<ElementPtr> m_roots;


};

}