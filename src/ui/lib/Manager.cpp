#include <ui/lib/Manager.h>

#include <utility>

#include <ui/lib/Generator.h>
#include <ui/lib/TransientMessage.h>
#include <ui/shared/ConsoleDialog.h>
#include <graphics/RenderInterface.h>
#include <graphics/Primatives.h>
#include <resource/ResourceManager.h>
#include <utils/GlobalConfig.h>
#include <engine/LuaState.h>

using namespace UI;

Manager::Manager(LuaState& luaState)
    : m_lua(luaState), m_isMidDrag(false), m_hasModalDialog(false), m_generator(this, luaState)
{
    auto windowSize = ResourceManager::get().getWindow()->getSize();
    m_modalSprite = createRectangle( windowSize, Colour::Black.withAlpha(150) );

    loadScripts();
}

bool Manager::input(IEvent &evt)
{
    bool out = false;
    
    switch ( evt.type )
    {
        case IEventType::KeyPress:
            out = handleKeyPress(evt.keyPress);
            break;
        case IEventType::MouseDown:
            out = handleMouseDown(evt.mouseDown);
            break;
        case IEventType::MouseUp:
            out = handleMouseUp(evt.mouseUp);
            break;
        case IEventType::MouseMove:
            out = handleMouseMove(evt.mouseMove);
            break;
        case IEventType::ScrollWheel:
            out = handleScrollwheel(evt.scrollWheel);
            break;
        case IEventType::TextInput:
            out = handleTextInput(evt.textInput);
            break;
        case IEventType::WindowResize:
            break;
    }

    // Stop all events from getting to the level while a modal dialog is open
    if ( m_hasModalDialog && evt.type != IEventType::KeyPress )
    {
        return true;
    }

    return out;
}

void Manager::update(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    // Update all of our non-modal dialogs
    for ( auto const& r: m_roots )
    {
        if ( !r->isModal() )
        {
            r->update(ticks, iinter, rInter);
        }
    }
    
    // If we have a modal dialog, render the background overlay sprite and then render the modal dialog
    if ( m_hasModalDialog )
    {
        auto modal = m_modalDialog.lock();
        rInter.addItem( m_modalSprite.renderObject({0, 0}), RenderLayer::UI);
        modal->update(ticks, iinter, rInter);
    }

    // Render any of our elements which have been queued up to delete
    for ( auto &elem : m_delayedDeleteElems )
    {
        deleteElement(elem);
    }
    
    m_delayedDeleteElems.clear();
}

void Manager::doLayout()
{
    auto windowSize = ResourceManager::get().getWindow()->getSize();

    // Set the position of any elements whose position is relative to the current window size
    for ( auto const& wa : m_windowAlignments )
    {
        auto pos = alignRectWithinRect( windowSize, wa.element->outerBounds().right(), wa.alignment );
        pos += wa.offset;
        wa.element->setLocalPosition(pos);
    }

    // Set the position of any elements whose position is relative to other elements
    for ( auto const& ea : m_elementAlignments )
    {
        auto depBounds = ea.dependent->outerBounds();
        auto fixedBounds = ea.target->outerBounds();

        auto pos = alignRectToRect( depBounds.right(), fixedBounds.right(), ea.alignment );
        pos += ea.offset;

        ea.dependent->setLocalPosition( ea.target->globalPosition() + pos );
    }
}

void Manager::deleteElement(Element* element)
{
    if ( !element )
        return;
    
    // If this element is modal, remove our modal overlay along with the element
    if ( element->isModal() )
    {
        m_modalDialog.reset();
        m_hasModalDialog = false;
    }
    
    if ( element->parent() )
    {
        element->parent()->removeChild(element);
    }
    else
    {
        m_roots.erase( std::remove_if( m_roots.begin(), m_roots.end(), [&]( auto const& item ){
            return item.get() == element;
        }), m_roots.end() );
    }
}

void Manager::deleteElement( std::shared_ptr<Element> element )
{
    deleteElement( element.get() );
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
                    // Iterate in reverse order - children are drawn front to back so we want the top child
                    // in case of overlapping children
                    for (auto it = curr->children().rbegin(); it != curr->children().rend(); it++)
                    {
                        if ( (*it)->outerBounds().contains(pos) )
                        {
                            next = *it;
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
    m_lastMouseDownPos = evt.screenPos;

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
        
        m_lastMouseDownPos = evt.screenPos;
        return true;
    }
}

bool Manager::handleMouseUp(IEventMouseUp evt)
{
    if (m_isMidDrag)
    {
        UEvent uevent;
        uevent.type = UEventType::DragStop;
        uevent.targetElement = m_mouseDownElem;
        uevent.dragEvent.currPos = {0, 0};
        uevent.dragEvent.originalTarget = m_mouseDownElem.get();
        uevent.dragEvent.startPos = *m_lastMouseDownPos;
        
        m_mouseDownElem->acceptEvent(uevent);
        m_isMidDrag = false;
    }
    
    m_lastMouseDownPos.reset();
    
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
    // Handle dragging if necessary

    if (m_lastMouseDownPos.has_value() && m_mouseDownElem)
    {
        if (m_isMidDrag)
        {
            // We are mid drag
            UEvent uevent;
            uevent.type = UEventType::DragMove;
            uevent.targetElement = m_mouseDownElem;
            uevent.dragEvent.currPos = evt.screenPos;
            uevent.dragEvent.originalTarget = m_mouseDownElem.get();
            uevent.dragEvent.startPos = *m_lastMouseDownPos;
            
            m_mouseDownElem->acceptEvent(uevent);
        }
        else
        {
            // We were *not* mid drag, and need to start a new drag
            // We are mid drag
            UEvent uevent;
            uevent.type = UEventType::DragStart;
            uevent.targetElement = m_mouseDownElem;
            uevent.dragEvent.currPos = evt.screenPos;
            uevent.dragEvent.originalTarget = m_mouseDownElem.get();
            uevent.dragEvent.startPos = *m_lastMouseDownPos;
            m_mouseDownElem->acceptEvent(uevent);
            
            m_isMidDrag = true;
        }
    }

    // Handle everything else

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
        
        // Close any tooltips that are open
        closeTooltip();
    }

    if ( !enters.empty() )
    {
        UEvent enterEvent;
        enterEvent.type = UEventType::MouseIn;
        enterEvent.mouseMoveEvent.pos = evt.screenPos;

        ElementPtr tooltipSpawn;

        for ( auto const& w: enters )
        {
            enterEvent.targetElement = w;
            w->acceptEvent(enterEvent);
            
            if (w->hasTooltipSpawner())
            {
                tooltipSpawn = w;
            }
        }
    
        if (tooltipSpawn)
        {
            auto tooltip = tooltipSpawn->generateTooltip();
            if (tooltip)
            {
                openTooltip(tooltip, tooltipSpawn->outerBounds());
            }
        }
    }

    return !m_hoveredElems.empty();
}

bool Manager::handleKeyPress(IEventKeyPress evt)
{
    // TODO: Real hotkey map
    if ( evt.keyCode == SDLK_BACKQUOTE )
    {
        toggleConsoleWindow();
        return true;
    }
    
    UEvent uevent;
    uevent.type = UEventType::KeyPress;
    uevent.keyEvent.keyCode = evt.keyCode;
    
    auto focused = m_focusedElement.lock();
    if ( focused )
    {
        focused->acceptEvent( uevent );
        return true;
    }
    
    if ( m_hasModalDialog )
    {
        auto modal = m_modalDialog.lock();
        if ( modal->hasHotkey(evt.keyCode) )
        {
            modal->acceptEvent(uevent);
            return true;
        }
    }
   
    return false;
}

bool Manager::handleScrollwheel(IEventScrollWheel evt)
{
    // Get all the elements under the mouse cursor
    auto elems = windowsAtPoint(evt.screenPos);

    if ( elems.empty() )
    {
        return false;
    }
    else
    {
        UEvent uevent;
        uevent.type = UEventType::ScrollWheel;
        uevent.mouseScrollEvent.screenPos = evt.screenPos;
        uevent.mouseScrollEvent.magnitude = evt.magnitude;

        for ( auto it = elems.rbegin(); it != elems.rend(); it++ )
        {
            (*it)->acceptEvent(uevent);
        }

        return true;
    }


    return false;
}

bool Manager::handleTextInput( IEventTextInput evt )
{
    auto focused = m_focusedElement.lock();
    if ( focused )
    {
        UEvent uevent;
        uevent.type = UEventType::TextInput;
        memset( uevent.textInputEvent.text, 0, 32 );
        strncpy_s( uevent.textInputEvent.text, evt.text, 31 );
        focused->acceptEvent( uevent );
    }

    return false;
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

void Manager::openTooltip( std::shared_ptr<Tooltip> const &data, RectI const &spawner )
{
    // Never want more than one tooltip
    closeTooltip();

    int offset = 4;
    m_tooltip = data;
    Vector2i tooltipSize = m_tooltip->outerBounds().right();
    
    // Default to the top right of the spawning rectangle
    Vector2i tooltipPos = spawner.left() + Vector2i{ spawner.w(), -tooltipSize.y() } + Vector2i{ offset, -offset };
    
    auto windowSize = ResourceManager::get().getWindow()->getSize();
    
    if ( tooltipPos.x() + tooltipSize.x() > windowSize.x() )
    {
        // Tooltip falls off the end of the screen - move it to the left of the spawner
        tooltipPos = { spawner.x() - tooltipSize.x() - offset, tooltipPos.y() };
    }
    if ( tooltipPos.y() - tooltipSize.y() < 0 )
    {
        // Tooltip falls off of the top of the screen - move it to underneath the spawner
        tooltipPos = { tooltipPos.x(), spawner.y() + spawner.h() + offset };
    }
    
    m_tooltip->setLocalPosition( tooltipPos );
//    m_tooltip->setFadeIn();
}

void Manager::closeTooltip()
{
    if (m_tooltip)
    {
        deleteElement( m_tooltip.get() );
    }
}

void Manager::showSingleTileHighlight(Vector2i tile, SingleTileHighlightType type)
{
    if (m_tileHighlight)
    {
        deleteElement( m_tileHighlight.get() );
    }

    m_tileHighlight = createElement<UI::SingleTileHighlight>( nullptr, tile, type );
}

void Manager::removeSingleTileHighlight()
{
    if (m_tileHighlight)
    {
        deleteElement( m_tileHighlight.get() );
        m_tileHighlight.reset();
    }
}

void Manager::makeElementModal(std::shared_ptr<Element> elem)
{
    auto existing = m_modalDialog.lock();
    if (existing)
    {
        existing->setIsModal(false);
    }

    m_modalDialog = elem;
    m_hasModalDialog = true;
    elem->setIsModal(true);
}

void Manager::centreElementInWindow(const std::shared_ptr<Element> &element)
{
    auto size = element->outerBounds().right();
    auto wndSize = ResourceManager::get().getWindow()->getSize();

    element->setLocalPosition( (wndSize - size) / 2 );
}

bool Manager::hasModalDialog() const
{
    return m_hasModalDialog;
}

void Manager::delayedDeleteElement(Element* element)
{
    m_delayedDeleteElems.push_back(element);
}

void Manager::displayTransientMessage(std::string message, float displayTime )
{
    auto testTransient = createElement<UI::TransientMessage>( nullptr, 0.5f, displayTime, 0.8f );
    testTransient->getLabel()->setText( message );

    auto dlgSize = testTransient->outerBounds().right();
    auto wndSize = ResourceManager::get().getWindow()->getSize();
    testTransient->setLocalPosition({
            (wndSize.x() - dlgSize.x()) / 2,
            (wndSize.y() - dlgSize.y()) / 4
    });
}

ElementPtr Manager::generateFromLua( std::string const &name, Element *existing )
{
    return m_generator.fromLua( name, existing );
}

void Manager::toggleConsoleWindow()
{
    auto existing = m_consoleDialog.lock();
    if ( existing )
    {
        deleteElement( existing.get() );
        m_consoleDialog.reset();
    }
    else
    {
        auto console = createElement<ConsoleDialog>( nullptr );
        m_consoleDialog = console;
        
        auto wndSize = ResourceManager::get().getWindow()->getSize();
        float frac = 0.6f;
        int height = 100;
        
        console->setPreferredContentSize({ (int)(wndSize.x() * frac), 1 });
        console->setLocalPosition({ (int)((wndSize.x() * ( 1.0 -  frac ) / 2.0f)), wndSize.y() - 20 - height });
    }
}

void Manager::setFocusedElement( ElementPtr elem )
{
    m_focusedElement = elem;
}

void Manager::loadScripts()
{
    m_lua.state().unsafe_script( ResourceManager::get().getLuaScript( "ui/Manager" ).data() );

    m_lua.state()["ui_manager"] = this;

    for ( auto const& text : ResourceManager::get().getLuaScriptsInDir( "ui/elements" ) )
    {
//        m_lua.runScript( text->data() );
    }
}


WindowAlignment::WindowAlignment(ElementPtr element, Alignment alignment, Vector2i offset)
: element(std::move(element)), alignment(alignment), offset(offset) {}

ElementAlignment::ElementAlignment(ElementPtr dependent, ElementPtr target, Alignment alignment, Vector2i offset)
    : dependent(dependent), target(target), alignment(alignment), offset(offset) {}

void Manager::setLuaType(LuaState &lua)
{
    auto userType = lua.state().new_usertype<UI::Manager>(
        "Manager"
    );

//    userType["deleteElement"] = &UI::Manager::deleteElement;
    userType["withId"] = &UI::Manager::withId<UI::Element>;
    userType["createElement"] = &UI::Manager::createElementBlank;
}


