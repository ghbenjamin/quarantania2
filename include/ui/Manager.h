#pragma once

#include <vector>

#include <ui/Element.h>
#include <ui/Layout.h>
#include <ui/UEvent.h>
#include <ui/ContextMenu.h>
#include <engine/InputInterface.h>
#include <ui/Tooltips.h>
#include "TileHighlights.h"

struct IEvent;
class Level;
class InputInterface;
class RenderInterface;

namespace UI
{

struct WindowAlignment
{
    WindowAlignment(ElementPtr element, Alignment alignment, Vector2i offset);
    ~WindowAlignment() = default;

    ElementPtr element;
    UI::Alignment alignment;
    Vector2i offset;
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
        auto ptr = std::make_shared<WT>( this, parent, std::forward<Args>(args)... );
        if ( parent == nullptr )
        {
            m_roots.push_back(ptr);
        }
        else
        {
            parent->addChild(ptr);
        }

        return ptr;
    }

    void deleteElement( const ElementPtr& element );

    void alignElementToWindow( const ElementPtr& element, UI::Alignment alignment, Vector2i offset);
    void unalignElementToWindow( ElementPtr element );

    template <typename T = Element>
    std::shared_ptr<T> withId( std::string const& id )
    {
        auto ptr = firstElementMatching([&](auto const& e){
            return e->id() == id;
        });

        return ptr->asType<T>();
    }

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

    void openTooltip( TooltipData const& data, Vector2i pos, bool longContent = false );
    void openTooltip( std::vector<TooltipData> const& data, Vector2i pos, bool longContent = false );
    void closeTooltip();

    void showSingleTileHighlight(Vector2i tile, SingleTileHighlightType type);
    void removeSingleTileHighlight();


private:
    bool handleMouseMove( IEventMouseMove evt );
    bool handleMouseDown( IEventMouseDown evt );
    bool handleMouseUp( IEventMouseUp evt );


    std::vector<ElementPtr> m_roots;
    std::vector<WindowAlignment> m_windowAlignments;

    std::shared_ptr<SingleTileHighlight> m_tileHighlight;

    ElementList m_hoveredElems;
    ElementPtr m_mouseDownElem;

    Level* m_level;
};

}