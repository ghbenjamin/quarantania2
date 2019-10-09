#pragma once

#include <ui/Element.h>

class Level;

namespace UI
{

class MinimapView : public Element
{
public:
    MinimapView(Level* level);
    ~MinimapView() override = default;

private:
    void updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;
    void onMoveSelf() override;
    void onSizeSelf() override;

    void doRender();

private:
    Vector2i m_viewport; // How big is the viewport into the minimap?
    Vector2i m_playerPos; // What is the tile position of the player?

    Sprite m_rendered;  // Final rendered minimap
    Level* m_level;

};


}

