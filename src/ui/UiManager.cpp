#include <ui/UiManager.h>

#include <graphics/RenderInterface.h>
#include <game/InputInterface.h>
#include <utils/Logging.h>
#include <resource/ResourceManager.h>

#include <utility>

using namespace UI;

bool UiManager::input(IEvent &evt)
{
    switch ( evt.type )
    {
        case IEventType::KeyPress:
            break;
        case IEventType::MouseDown:
            handleMouseDown(evt.mouseDown);
            break;
        case IEventType::MouseUp:
            handleMouseUp(evt.mouseUp);
            break;
        case IEventType::MouseMove:
            handleMouseMove(evt.mouseMove);
            break;
        case IEventType::WindowResize:
            break;
    }

    return false;
}

void UiManager::update(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    for ( auto const& r: m_roots )
    {
        r->update(ticks, iinter, rInter);
    }
}

void UiManager::doLayout()
{
    auto windowSize = ResourceManager::get().getWindow()->getSize();

    for ( auto const& wa : m_windowAlignments )
    {
        auto pos = alignRectWithinRect( windowSize, wa.element->outerSize(), wa.alignment );
        wa.element->setLocalPosition(pos);
    }
}

void UiManager::deleteElement(ElementPtr element)
{
    AssertAlways();
}

void UiManager::alignElementToWindow(const ElementPtr& element, UI::Alignment alignment, int offset)
{
    unalignElementToWindow( element );

    m_windowAlignments.emplace_back( element, alignment, offset );

    doLayout();
}

void UiManager::unalignElementToWindow(ElementPtr element)
{
    m_windowAlignments.erase( std::remove_if( m_windowAlignments.begin(), m_windowAlignments.end(),
          [&](auto const& item) {
              return item.element == element;
          }), m_windowAlignments.end()
    );
}

ElementPtr UiManager::withId(std::string const &id)
{
    auto out = ElementPtr();

    for ( auto& r : m_roots )
    {
        if ( r->id() == id )
        {
            out = r;
            break;
        }
        else
        {
            auto ptr = r->descWithId(id);
            if (ptr)
            {
                out = ptr;
                break;
            }
        }
    }

    return out;
}

UI::ElementList UiManager::windowsAtPoint(Vector2i pos) const
{
    std::vector<ElementPtr> out;
    ElementPtr curr;
    ElementPtr next;

    for ( auto const& root : m_roots )
    {
        if ( root->bounds().contains(pos) )
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
                        if ( child->bounds().contains(pos) )
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
UiManager::partitionWindowLists(ElementList const &lhs, ElementList const &rhs) const
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

bool UiManager::handleMouseDown(IEventMouseDown evt)
{
    auto elems = windowsAtPoint(evt.screenPos);

    if ( elems.empty() )
    {
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

bool UiManager::handleMouseUp(IEventMouseUp evt)
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

bool UiManager::handleMouseMove(IEventMouseMove evt)
{
    auto elems = windowsAtPoint(evt.screenPos);
    auto [exits, enters] = partitionWindowLists(m_hoveredElems, elems);

    m_hoveredElems = elems;

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

    return true;
}

WindowAlignment::WindowAlignment(ElementPtr element, Alignment alignment, int offset)
: element(std::move(element)), alignment(alignment), offset(offset)
{}
