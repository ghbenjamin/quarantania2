#include <ui/Manager.h>

#include <graphics/RenderInterface.h>
#include <engine/InputInterface.h>
#include <utils/Logging.h>
#include <resource/ResourceManager.h>

#include <utility>
#include <utils/GlobalConfig.h>

using namespace UI;

Manager::Manager(Level *level)
        : m_level(level) { }

bool Manager::input(IEvent &evt)
{
    switch ( evt.type )
    {
        case IEventType::KeyPress:
            break;
        case IEventType::MouseDown:
            return handleMouseDown(evt.mouseDown);
            break;
        case IEventType::MouseUp:
            return handleMouseUp(evt.mouseUp);
            break;
        case IEventType::MouseMove:
            return handleMouseMove(evt.mouseMove);
            break;
        case IEventType::WindowResize:
            break;
    }

    return false;
}

void Manager::update(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    for ( auto const& r: m_roots )
    {
        r->update(ticks, iinter, rInter);
    }
}

void Manager::doLayout()
{
    auto windowSize = ResourceManager::get().getWindow()->getSize();

    for ( auto const& wa : m_windowAlignments )
    {
        auto pos = alignRectWithinRect( windowSize, wa.element->outerBounds().right(), wa.alignment );
        pos += wa.offset;
        wa.element->setLocalPosition(pos);
    }

    for ( auto const& ea : m_elementAlignments )
    {
        auto depBounds = ea.dependent->outerBounds();
        auto fixedBounds = ea.target->outerBounds();

        auto pos = alignRectToRect( depBounds.right(), fixedBounds.right(), ea.alignment );
        pos += ea.offset;

        ea.dependent->setLocalPosition( ea.target->globalPosition() + pos );
    }
}

void Manager::deleteElement(const ElementPtr& element)
{
    if ( !element )
        return;

    if ( element->parent() )
    {
        element->parent()->removeChild(element);
    }
    else
    {
        m_roots.erase( std::remove(m_roots.begin(), m_roots.end(), element), m_roots.end() );
    }
}

void Manager::alignElementToWindow(const ElementPtr& element, UI::Alignment alignment, Vector2i offset)
{
    unalignElementFromWindow(element);
    m_windowAlignments.emplace_back( element, alignment, offset );

    doLayout();
}

void Manager::alignElementToElement(const ElementPtr &desc, ElementPtr const& target, UI::Alignment alignment, Vector2i offset)
{
    unalignElements(desc, target);
    m_elementAlignments.emplace_back( desc, target, alignment, offset );

    doLayout();
}

void Manager::unalignElementFromWindow(ElementPtr element)
{
    m_windowAlignments.erase( std::remove_if( m_windowAlignments.begin(), m_windowAlignments.end(),
          [&](auto const& item) {
              return item.element == element;
          }), m_windowAlignments.end()
    );
}

void Manager::unalignElements(const ElementPtr &desc, const ElementPtr &target)
{
    m_elementAlignments.erase( std::remove_if( m_elementAlignments.begin(), m_elementAlignments.end(),
          [&](ElementAlignment const& item) {
              return item.target == target && item.dependent == desc;
          }), m_elementAlignments.end()
    );
}

UI::ElementList Manager::windowsAtPoint(Vector2i pos) const
{
    std::vector<ElementPtr> out;
    ElementPtr curr;
    ElementPtr next;

    for ( auto const& root : m_roots )
    {
        if ( root->outerBounds().contains(pos) )
        {
            curr = root;
            while ( curr )
            {
                out.push_back(curr);

                if ( !curr->hasChildren() )
                {
                    break;
                }
                else
                {
                    for ( auto const& child : curr->children() )
                    {
                        if ( child->outerBounds().contains(pos) )
                        {
                            next = child;
                            break;
                        }
                    }
                }

                if ( next )
                {
                    curr = next;
                    next = ElementPtr();
                }
                else
                {
                    break;
                }
            }

            break;
        }
    }

    // Ignore decorative elements
    out.erase( std::remove_if( out.begin(), out.end(), [](auto const& item){
        return item->isDecorative();
    }), out.end());

    return out;
}

std::tuple<ElementList, ElementList>
Manager::partitionWindowLists(ElementList const &lhs, ElementList const &rhs) const
{
    auto it_lhs = lhs.begin();
    auto it_rhs = rhs.begin();

    for ( ; it_lhs != lhs.end() && it_rhs != rhs.end(); it_lhs++, it_rhs++ )
    {
        if ( *it_lhs != *it_rhs )
        {
            break;
        }
    }

    auto exits = ElementList{ it_lhs, lhs.end() };
    auto enters = ElementList{ it_rhs, rhs.end() };

    return std::make_tuple( exits, enters );
}

bool Manager::handleMouseDown(IEventMouseDown evt)
{
    // Get all the elements under the mouse cursor
    auto elems = windowsAtPoint(evt.screenPos);

    if ( elems.empty() )
    {
        // If there's an open context menu (which we didn't click on), close it.
        // TODO Also close the menu if we click on a different ui item

        auto cm = firstElementMatching([](auto const& e){
            return e->id() == "context-menu";
        });

        if (cm)
        {
            deleteElement(cm);
        }

        return false;
    }
    else
    {
        UEvent uevent;
        uevent.type = UEventType::MouseDown;
        uevent.targetElement = elems.back();
        uevent.mouseButtonEvent.button = evt.button;
        uevent.mouseButtonEvent.pos = evt.screenPos;

        elems.back()->acceptEvent(uevent);

        m_mouseDownElem = elems.back();
        return true;
    }
}

bool Manager::handleMouseUp(IEventMouseUp evt)
{
    auto elems = windowsAtPoint(evt.screenPos);

    if ( elems.empty() )
    {
        m_mouseDownElem = ElementPtr();
        return false;
    }
    else
    {
        UEvent uevent;
        uevent.type = UEventType::MouseUp;
        uevent.targetElement = elems.back();
        uevent.mouseButtonEvent.button = evt.button;
        uevent.mouseButtonEvent.pos = evt.screenPos;

        elems.back()->acceptEvent(uevent);

        if ( elems.back() == m_mouseDownElem )
        {
            UEvent clickEvent;
            clickEvent.type = UEventType::Click;
            clickEvent.mouseButtonEvent.button = evt.button;
            clickEvent.mouseButtonEvent.pos = evt.screenPos;

            for ( auto rit = elems.rbegin(); rit != elems.rend(); rit++ )
            {
                clickEvent.targetElement = *rit;
                (*rit)->acceptEvent(clickEvent);

                if (uevent.stopPropagation)
                {
                    break;
                }
            }
        }

        m_mouseDownElem = ElementPtr();
        return true;
    }
}

bool Manager::handleMouseMove(IEventMouseMove evt)
{
    auto elems = windowsAtPoint(evt.screenPos);
    auto [exits, enters] = partitionWindowLists(m_hoveredElems, elems);

    m_hoveredElems = elems;

    UEvent moveEvent;
    moveEvent.type = UEventType::MouseMove;
    moveEvent.mouseMoveEvent.pos = evt.screenPos;

    for ( auto const& w: elems )
    {
        moveEvent.targetElement = w;
        w->acceptEvent(moveEvent);
    }

    if ( !exits.empty() )
    {
        UEvent exitEvent;
        exitEvent.type = UEventType::MouseOut;
        exitEvent.mouseMoveEvent.pos = evt.screenPos;

        for ( auto const& w: exits )
        {
            exitEvent.targetElement = w;
            w->acceptEvent(exitEvent);
        }
    }

    if ( !enters.empty() )
    {
        UEvent enterEvent;
        enterEvent.type = UEventType::MouseIn;
        enterEvent.mouseMoveEvent.pos = evt.screenPos;

        for ( auto const& w: enters )
        {
            enterEvent.targetElement = w;
            w->acceptEvent(enterEvent);
        }
    }

    return !m_hoveredElems.empty();
}

void Manager::openContextMenu(ContextMenuList const &items, Vector2i pos, ContextMenuCallback callback)
{
    auto cmenu = createElement<UI::ContextMenu>(nullptr, items, callback);
    cmenu->setLocalPosition( pos );
}

void Manager::cancelContextMenu()
{
    deleteElement( withId( "context-menu" ) );
}

Level *Manager::level()
{
    return m_level;
}

void Manager::openTooltip( TooltipData const& data, Vector2i pos, bool longContent )
{
    auto tooltip = createElement<Tooltip>( nullptr, data, longContent );
    tooltip->setLocalPosition( pos );
}

void Manager::openTooltip(std::vector<TooltipData> const &data, Vector2i pos, bool longContent )
{
    auto tooltip = createElement<Tooltip>( nullptr, data, longContent );
    tooltip->setLocalPosition( pos );
}

void Manager::closeTooltip()
{
    deleteElement( withId( "tooltip" ) );
}

void Manager::showSingleTileHighlight(Vector2i tile, SingleTileHighlightType type)
{
    if (m_tileHighlight)
    {
        deleteElement(m_tileHighlight);
    }

    m_tileHighlight = createElement<UI::SingleTileHighlight>( nullptr, tile, type );
}

void Manager::removeSingleTileHighlight()
{
    deleteElement(m_tileHighlight);
    m_tileHighlight.reset();
}

WindowAlignment::WindowAlignment(ElementPtr element, Alignment alignment, Vector2i offset)
: element(std::move(element)), alignment(alignment), offset(offset) {}

ElementAlignment::ElementAlignment(ElementPtr dependent, ElementPtr target, Alignment alignment, Vector2i offset)
    : dependent(dependent), target(target), alignment(alignment), offset(offset) {}
