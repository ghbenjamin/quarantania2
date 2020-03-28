#pragma once

#include <ui/Element.h>

namespace UI
{

class MinimapView : public Element
{
public:
     MinimapView(Manager* manager, Element* parent, TexturePtr texture);
     ~MinimapView() override = default;

private:
    void onMoveSelf() override;
    void onSizeSelf() override;

    void updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;

    void resetTexturePosition();

    TexturePtr m_texture;
    Vector2i m_viewSize;
    Vector2i m_viewOffset;

    RenderObject m_renderObj;
};


}