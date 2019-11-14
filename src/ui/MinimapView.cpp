#include <ui/MinimapView.h>
#include <game/Level.h>
#include <graphics/RenderInterface.h>
#include <graphics/Primatives.h>

UI::MinimapView::MinimapView(Level* level)
    : m_level(level), m_viewport(Vector2i{0, 0})
{
}

void UI::MinimapView::doRender()
{
    if ( m_viewport == Vector2i{0, 0} )
        return;

    m_rendered = createRectangle( m_viewport, Colour::Black );
}

void UI::MinimapView::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    if ( m_rendered )
    {
        rInter.addScreenItem( m_rendered.renderObject( globalPosition() ) );
    }
}

void UI::MinimapView::onMoveSelf()
{
    doRender();
}

void UI::MinimapView::onSizeSelf()
{
    m_viewport = contentSize();
    doRender();
}
