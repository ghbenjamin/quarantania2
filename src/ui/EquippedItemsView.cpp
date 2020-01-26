#include <ui/EquippedItemsView.h>
#include <resource/ResourceManager.h>
#include <graphics/RenderInterface.h>

UI::EquippedItemsView::EquippedItemsView()
{
    static const std::string SpritesheetName = "game-icons";

    auto addSlot = [this](auto&& slotType, auto&& spriteName, auto&& offset){
        static const std::string SpritesheetName = "game-icons";

        m_slotData.push_back({
                slotType, true,
                ResourceManager::get().getSprite(SpritesheetName, spriteName),
                offset,
                Vector2i{0, 0}
        });

        m_slotData.back().sprite.setRenderLayer(RenderLayer::UI);
    };

    addSlot(EquippedItemSlotType::Head,      "cowled",          Vector2i{16, 0});
    addSlot(EquippedItemSlotType::Headband,  "headband-knot",   Vector2i{52, 0});
    addSlot(EquippedItemSlotType::Eyes,      "spectacles",      Vector2i{88, 0});

    addSlot(EquippedItemSlotType::Shoulders, "pauldrons",       Vector2i{16, 36});
    addSlot(EquippedItemSlotType::Neck,      "necklace",        Vector2i{52, 36});
    addSlot(EquippedItemSlotType::Face,      "helmet",          Vector2i{88, 36});

    addSlot(EquippedItemSlotType::Chest,     "shirt",           Vector2i{16, 72});
    addSlot(EquippedItemSlotType::Body,      "robe",            Vector2i{52, 72});
    addSlot(EquippedItemSlotType::Armour,    "mail-shirt",      Vector2i{88, 72});

    addSlot(EquippedItemSlotType::MainHand,  "sword-hilt",      Vector2i{0, 108});
    addSlot(EquippedItemSlotType::Belt,      "belt",            Vector2i{36, 108});
    addSlot(EquippedItemSlotType::Wrists,    "gloves",          Vector2i{72, 108});
    addSlot(EquippedItemSlotType::OffHand,   "sword-hilt",      Vector2i{108, 108});

    addSlot(EquippedItemSlotType::Ring1,     "ring",            Vector2i{16, 144});
    addSlot(EquippedItemSlotType::Feet,      "boots",           Vector2i{52, 144});
    addSlot(EquippedItemSlotType::Ring2,     "ring",            Vector2i{88, 144});
}

void UI::EquippedItemsView::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    for ( auto const& item : m_slotData )
    {
        if (item.visible)
        {
            rInter.addScreenItem(item.sprite.renderObject(item.pos));
        }
    }
}

void UI::EquippedItemsView::onSizeSelf()
{
    layoutSlots();
}

void UI::EquippedItemsView::onMoveSelf()
{
    layoutSlots();
}

void UI::EquippedItemsView::layoutSlots()
{
    auto begin = globalPosition() + contentOffset();

    int leftOffset = innerBounds().w() / 2 - 66;
    int topOffset = 8;

    begin += {leftOffset, topOffset};

    for ( auto& s : m_slotData )
    {
        s.pos = begin + s.offset;
    }
}
