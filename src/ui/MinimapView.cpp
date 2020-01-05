#pragma once

#include <ui/MinimapView.h>
#include <graphics/RenderInterface.h>

UI::MinimapView::MinimapView(TexturePtr texture, Vector2i viewportSize)
: m_texture(texture), m_viewSize(viewportSize), m_viewOffset{0, 0}
{
    m_renderObj.texture = texture->raw();
    m_renderObj.renderLayer = RenderLayer::UI;
    m_renderObj.sourceRect = m_texture->sourceRect();

    auto const& txSize = m_texture->size();
    int actualX = std::min( txSize.x(), m_viewSize.x() );
    int actualY = std::min( txSize.y(), m_viewSize.y() );

    m_renderObj.targetRect = {0, 0, actualX, actualY};
}

void UI::MinimapView::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    rInter.addScreenItem( m_renderObj );
}

void UI::MinimapView::onMoveSelf()
{
    auto size = contentSize();
    int offset = size.x() / 2 - m_renderObj.targetRect.w / 2;

    m_renderObj.targetRect.x = bounds().x() + offset;
    m_renderObj.targetRect.y = bounds().y() + 10; // TODO Fix hacky offset
}
