#pragma once

#include <vector>

#include <ui/lib/Element.h>
#include <ui/lib/Layout.h>
#include <ui/lib/UEvent.h>
#include <ui/lib/ContextMenu.h>
#include <engine/InputInterface.h>
#include <ui/lib/Tooltips.h>
#include <ui/level/TileHighlights.h>

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

struct ElementAlignment
{
    ElementAlignment(ElementPtr dependent, ElementPtr target, Alignment alignment, Vector2i offset);
    ~ElementAlignment() = default;

    ElementPtr dependent;
    ElementPtr target;
    UI::Alignment alignment;
    Vector2i offset;
};


class Manager
{
public:

    Manager();
    ~Manager() = default;

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

    template <typename ...Args>
    std::shared_ptr<Element> createElement( Element* parent, Args...args )
    {
        return createElement<Element>( parent, std::forward<Args>(args)... );
    }

    void deleteElement( const ElementPtr& element );
    void delayedDeleteElement( const ElementPtr& element );

    void alignElementToWindow( const ElementPtr& element, UI::Alignment alignment, Vector2i offset);
    void alignElementToElement( ElementPtr const& desc, ElementPtr const& target, UI::Alignment alignment, Vector2i offset);

    void centreElementInWindow( std::shared_ptr<Element> const& element );

    void unalignElementFromWindow(ElementPtr element );
    void unalignElements( ElementPtr const& desc, ElementPtr const& target );

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
            tmp = r->firstDescMatchingCondition(callable);
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

    void openTooltip( std::shared_ptr<Tooltip> const &tooltip, RectI const& spawner );
    void closeTooltip();

    void showSingleTileHighlight(Vector2i tile, SingleTileHighlightType type);
    void removeSingleTileHighlight();

    void makeElementModal( std::shared_ptr<Element> elem );
    bool hasModalDialog() const;

    void displayTransientMessage( std::string message, float displayTime );

    template <typename T, typename ... Args>
    void toggleExclusiveDialog( std::string const& name, Args... args )
    {
        bool shouldCreate;
        bool shouldDelete;
    
        if ( !m_exclusiveDialogName.has_value() )
        {
            // There is no exclusive dialog - create one
            shouldCreate = true;
            shouldDelete = false;
        }
        else
        {
            if (name == *m_exclusiveDialogName)
            {
                // We're toggling the dialog which already exists. Destroy the existing one and don't create another
                shouldCreate = false;
                shouldDelete = true;
            }
            else
            {
                // We're toggling a different dialog - clear the existing one and open a new one
                shouldCreate = true;
                shouldDelete = true;
            }
        }
        
        
        if ( shouldDelete )
        {
            deleteElement( m_exclusiveDialog.lock() );
            m_exclusiveDialog.reset();
            m_exclusiveDialogName.reset();
        }
        
        if ( shouldCreate )
        {
            auto newDialog = createElement<T>( nullptr, std::forward<Args>(args)... );
            makeElementModal( newDialog );
            centreElementInWindow( newDialog );
            
            m_exclusiveDialog = newDialog;
            m_exclusiveDialogName = name;
        }
    }

private:
    bool handleMouseMove( IEventMouseMove evt );
    bool handleMouseDown( IEventMouseDown evt );
    bool handleMouseUp( IEventMouseUp evt );
    bool handleKeyPress( IEventKeyPress evt );
    bool handleScrollwheel( IEventScrollWheel evt );

    // Our root UI elements.
    std::vector<ElementPtr> m_roots;

    // Registry of which UI elements need to be kept in place relative to the window or to other UI elements
    std::vector<WindowAlignment> m_windowAlignments;
    std::vector<ElementAlignment> m_elementAlignments;

    // Elements which there need to be only one of
    std::shared_ptr<SingleTileHighlight> m_tileHighlight;
    std::shared_ptr<Tooltip> m_tooltip;
    
    std::weak_ptr<Element> m_exclusiveDialog;
    std::optional<std::string> m_exclusiveDialogName;
    
    ElementList m_hoveredElems;
    ElementPtr m_mouseDownElem;
    
    bool m_isMidDrag;
    std::optional<Vector2i> m_lastMouseDownPos;
    
    std::weak_ptr<Element> m_modalDialog;
    Sprite m_modalSprite;
    bool m_hasModalDialog;

    std::vector<ElementPtr> m_delayedDeleteElems;
};

}