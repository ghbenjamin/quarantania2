#pragma once

#include <ui/Element.h>
#include <ui/Defines.h>
#include <resource/Font.h>
#include <resource/Texture.h>
#include <graphics/RenderObject.h>

namespace UI
{

class TextNode : public Element
{
public:
    TextNode(Manager* manager, Element* parent);
    explicit TextNode(Manager* manager, Element* parent, TextStyle const& style);
    ~TextNode() override = default;

    void setText( std::string const& text );
    void clearText();

protected:
    void updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;

private:
    void onSizeSelf() override;
    void onMoveSelf() override;
    void renderText();

    FontPtr m_font;
    TexturePtr m_rendered;
    RenderObject m_renderObject;
    std::string m_data;
    TextStyle m_style;
};

}
