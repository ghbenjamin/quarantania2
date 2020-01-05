#pragma once

#include <ui/Element.h>

namespace UI
{

class MinimapView : public Element
{
public:
     MinimapView(TexturePtr texture, Vector2i viewportSize);
     ~MinimapView() override = default;

private:
    void onMoveSelf() override;

    void updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;


    TexturePtr m_texture;
    Vector2i m_viewSize;
    Vector2i m_viewOffset;

    RenderObject m_renderObj;
};


}