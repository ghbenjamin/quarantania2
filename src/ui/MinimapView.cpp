#pragma once

#include <ui/MinimapView.h>
#include <graphics/RenderInterface.h>

UI::MinimapView::MinimapView(Manager* manager, Element* parent, TexturePtr texture)
: Element(manager, parent), m_texture(texture), m_viewOffset{0, 0}
{
    setId("minimap");

    m_renderObj.texture = texture->raw();
    m_renderObj.renderLayer = RenderLayer::UI;
    m_renderObj.sourceRect = m_texture->sourceRect();
    m_renderObj.targetRect = {0, 0, 0, 0};
}

void UI::MinimapView::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    rInter.addScreenItem( m_renderObj );
}

void UI::MinimapView::onMoveSelf()
{
    resetTexturePosition();
}

void UI::MinimapView::onSizeSelf()
{
    resetTexturePosition();
}

void UI::MinimapView::resetTexturePosition()
{
    auto actualSize = innerBounds().right();
    auto const& txSize = m_texture->size();

    int actualW = std::min( txSize.x(), actualSize.x() );
    int actualH = std::min( txSize.y(), actualSize.y() );

    int offsetX = 0;
    int offsetY = 0;

    if ( actualW < actualSize.x() )
    {
        offsetX = (actualSize.x() - actualW) / 2;
    }

    if ( actualH < actualSize.y() )
    {
        offsetY = (actualSize.y() - actualH) / 2;
    }

    m_renderObj.targetRect = {
            outerBounds().x() + contentOffset().x() + offsetX,
            outerBounds().y() + contentOffset().y() + offsetY,
            actualW,
            actualH
    };

    m_renderObj.sourceRect.w = actualW;
    m_renderObj.sourceRect.h = actualH;
}