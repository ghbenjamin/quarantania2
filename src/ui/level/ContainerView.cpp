#include <ui/level/ContainerView.h>
#include <ui/lib/Manager.h>
#include <graphics/Primatives.h>
#include <graphics/RenderInterface.h>
#include <components/ContainerComponent.h>
#include <resource/ResourceManager.h>
#include <game/Level.h>
#include <utils/GlobalConfig.h>

UI::ContainerView::ContainerView(Manager* manager, Element* parent, Vector2i iconDims)
: Element(manager, parent), m_iconDims(iconDims), m_iconSpacing(2)
{
    setLayout<FreeLayout>();

    m_emptySlot = ResourceManager::get().getSprite("ui-frames", "inventory-slot");
    m_emptySlot.setRenderLayer(RenderLayer::UI);

    auto [slotW, slotH] = m_emptySlot.size();
    setPreferredContentSize({
        slotW * m_iconDims.x() + m_iconSpacing * ( m_iconDims.x() - 1 ),
        slotH * m_iconDims.y() + m_iconSpacing * ( m_iconDims.y() - 1 )
    });

    addEventCallback(UEventType::Click, [this](UEvent& evt){
        onClick( evt.mouseButtonEvent );
    });
}

void UI::ContainerView::refresh(EntityRef entity)
{

}

void UI::ContainerView::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    Vector2i globalOffset = contentOffset() + globalPosition();
    auto [slotW, slotH] = m_emptySlot.size();
    slotW += m_iconSpacing;
    slotH += m_iconSpacing;

    // Render each of our positions - both the empty slot background and any items if present
    for (int j = 0; j < m_iconDims.y(); j++)
    {
        for (int i = 0; i < m_iconDims.x(); i++)
        {
            Vector2i offset = { i * slotW, j * slotH };
            rInter.addScreenItem( m_emptySlot.renderObject( offset + globalOffset ) );
        }
    }
}

void UI::ContainerView::onClick(UI::UMouseButtonEvent &evt)
{

}