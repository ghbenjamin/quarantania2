#include <ui/UiManager.h>

#include <graphics/RenderInterface.h>
#include <game/InputInterface.h>
#include <utils/Logging.h>
#include <resource/ResourceManager.h>

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
            onWindowResize();
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

void UiManager::onWindowResize()
{
    doLayout();
}

WindowAlignment::WindowAlignment(const ElementPtr &element, Alignment alignment, int offset)
: element(element), alignment(alignment), offset(offset)
{}
