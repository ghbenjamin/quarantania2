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
        case IEventType::MouseClick:
            break;
        case IEventType::MouseMove:
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
                                              }), m_windowAlignments.end());
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

WindowAlignment::WindowAlignment(ElementPtr element, Alignment alignment, int offset)
: element(std::move(element)), alignment(alignment), offset(offset)
{}
