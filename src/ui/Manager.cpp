#include <ui/Manager.h>

#include <graphics/RenderInterface.h>
#include <game/InputInterface.h>
#include <utils/Logging.h>
#include <resource/ResourceManager.h>

#include <utility>
#include <utils/GlobalConfig.h>

using namespace UI;

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
        wa.element->setLocalPosition(pos);
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

void Manager::alignElementToWindow(const ElementPtr& element, UI::Alignment alignment, int offset)
{
    unalignElementToWindow( element );

    m_windowAlignments.emplace_back( element, alignment, offset );

    doLayout();
}

void Manager::unalignElementToWindow(ElementPtr element)
{
    m_windowAlignments.erase( std::remove_if( m_windowAlignments.begin(), m_windowAlignments.end(),
          [&](auto const& item) {
              return item.element == element;
          }), m_windowAlignments.end()
    );
}

ElementPtr Manager::withId(std::string const &id)
{
    return firstElementMatching([&](auto const& e){
        return e->id() == id;
    });
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

    // Ignore the highlight elements
    elems.erase( std::remove_if( elems.begin(), elems.end(), [](auto const& item){
        return item->id() == "tile-highlight";
    }), elems.end());

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
    removeTileHighlight();

    auto cmenu = createElement<UI::ContextMenu>(nullptr, items, callback);
    cmenu->setLocalPosition( pos );
}

void Manager::addTileHighlight(Vector2i screenPos)
{
    auto elem = createElement<Element>(nullptr);
    elem->setPreferredContentSize({GlobalConfig::TileSizePx, GlobalConfig::TileSizePx});
    elem->setBackgroundSprite({"tile-ui", "green-brackets"});
    elem->setLocalPosition( screenPos );
    elem->setId("tile-highlight");
}

void Manager::removeTileHighlight()
{
    deleteElement( withId( "tile-highlight" ) );
}

void Manager::cancelContextMenu()
{
    deleteElement( withId( "context-menu" ) );
}

Manager::Manager(Level *level)
: m_level(level)
{
}

Level *Manager::level()
{
    return m_level;
}

WindowAlignment::WindowAlignment(ElementPtr element, Alignment alignment, int offset)
: element(std::move(element)), alignment(alignment), offset(offset)
{}
