#pragma once

#include <vector>

#include <ui/Element.h>
#include <ui/Layout.h>
#include <ui/UEvent.h>
#include <ui/ContextMenu.h>
#include <game/InputInterface.h>

struct IEvent;
class Level;
class InputInterface;
class RenderInterface;

namespace UI
{

struct WindowAlignment
{
    WindowAlignment(ElementPtr element, Alignment alignment, int offset);
    ~WindowAlignment() = default;

    ElementPtr element;
    UI::Alignment alignment;
    int offset;
};


class Manager
{
public:

    Manager(Level* level);
    ~Manager() = default;

    Level* level();
    bool input(IEvent &evt);
    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);

    void doLayout();

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

    void deleteElement( const ElementPtr& element );

    void alignElementToWindow( const ElementPtr& element, UI::Alignment alignment, int offset = 0);
    void unalignElementToWindow( ElementPtr element );

    ElementPtr withId( std::string const& id );

    template <typename Callable>
    ElementPtr firstElementMatching( Callable&& callable ) const
    {
        ElementPtr out = ElementPtr();
        ElementPtr tmp;

        for ( auto const& r : m_roots )
        {
            tmp = r->firstMatchingCondition(callable);
            if ( tmp )
            {
                out = tmp;
                break;
            }
        }

        return out;
    }

    ElementList windowsAtPoint( Vector2i pos ) const;
    std::tuple<ElementList, ElementList> partitionWindowLists( ElementList const& lhs, ElementList const& rhs ) const;


    // API

    void openContextMenu( ContextMenuList const& items, Vector2i pos, ContextMenuCallback callback);
    void cancelContextMenu();

    void addTileHighlight(Vector2i screenPos);
    void removeTileHighlight();

private:
    bool handleMouseMove( IEventMouseMove evt );
    bool handleMouseDown( IEventMouseDown evt );
    bool handleMouseUp( IEventMouseUp evt );

private:
    std::vector<ElementPtr> m_roots;
    std::vector<WindowAlignment> m_windowAlignments;

    ElementList m_hoveredElems;
    ElementPtr m_mouseDownElem;

    Level* m_level;
};

}