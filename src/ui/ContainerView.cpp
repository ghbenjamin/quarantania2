#include <ui/ContainerView.h>
#include <graphics/Primatives.h>
#include <graphics/RenderInterface.h>

UI::ContainerView::ContainerView()
: m_emptySlot( createRectangle( {IconSize, IconSize}, Colour::Black ) )
{
    setBackgroundColour({200, 200, 200, 255});
    m_emptySlot.setRenderLayer(RenderLayer::UI);
}

void UI::ContainerView::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    int idx = 0;
    Vector2i offset = globalPosition() + contentOffset();
    Vector2i curr = {PaddingThick, PaddingThick};

    for ( int j = 0; j < m_tileBounds.y(); j++ )
    {
        for ( int i = 0; i < m_tileBounds.x(); i++ )
        {
            if (idx < m_items.size())
            {
                rInter.addScreenItem( m_items[idx].sprite.renderObject(curr + offset) );
            }
            else
            {
                rInter.addScreenItem( m_emptySlot.renderObject(curr + offset) );
            }

            curr.x( curr.x() + IconSize + PaddingThick );
            idx++;
        }

        curr.y( curr.y() + IconSize + PaddingThick );
        curr.x( PaddingThick );
    }
}

void UI::ContainerView::onSizeSelf()
{
    rearrangeItems();
}

void UI::ContainerView::onMoveSelf()
{
    rearrangeItems();
}

void UI::ContainerView::rearrangeItems()
{
    m_tileBounds = {
        (innerBounds().w() - PaddingThick) / (IconSize + PaddingThick),
        (innerBounds().h() - PaddingThick) / (IconSize + PaddingThick)
    };
}

void UI::ContainerView::reimportItems()
{
    // ????
}


