#include <ui/lib/ScrollHolder.h>
#include <utils/Memory.h>

UI::ScrollHolder::ScrollHolder(UI::Manager *manager, UI::Element *parent)
        : Element(manager, parent), m_scrollPercentage(0.0f)
{
    setBackground( Colour::Pink.withAlphaF(0.4f) );

    auto layout = utils::make_unique_with_type<ElementLayout, VScrollLayout>(this );
    setLayout( layout );
    setBoundsScissoring(true);

    addEventCallback( UEventType::ScrollWheel, [this](UEvent const& evt) {
        this->onScroll( evt.mouseScrollEvent );
    });
}

void UI::ScrollHolder::setScrollPercentage(float percentage)
{
    m_scrollPercentage = percentage;

    if ( m_scrollPercentage > 1.0f )
    {
        m_scrollPercentage = 1.0f;
    }
    else if ( m_scrollPercentage < 0.0f )
    {
        m_scrollPercentage = 0.0f;
    }

    doLayout();
}

float UI::ScrollHolder::getScrollPercentage() const
{
    return m_scrollPercentage;
}

int UI::ScrollHolder::getScrollHeight() const
{
    return m_scrollHeight;
}

void UI::ScrollHolder::onScroll(UI::UMouseScrollEvent evt)
{
    setScrollPercentage(m_scrollPercentage - (float) evt.magnitude * 0.05f);
}
