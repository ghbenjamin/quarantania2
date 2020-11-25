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

    TexturePtr m_rendered;
    RenderObject m_renderObject;
    std::string m_data;
    TextStyle m_style;
};




class Button : public Element
{
public:
    Button( Manager* manager, Element* parent, std::string const& text, std::function<void()> const& callback );
    ~Button() override = default;

private:
    std::function<void()> m_callback;
};




class Label : public Element
{
public:
    Label(Manager* manager, Element* parent);
    ~Label() override = default;

    void setText( std::string const& text );

private:

    std::shared_ptr<TextNode> m_textNode;

};


class Icon : public Element
{
public:
    Icon(Manager* manager, Element* parent, Sprite const& img);
    Icon(Manager* manager, Element* parent, SpritesheetKey const& img);
    ~Icon() override = default;

protected:
    void updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;

private:
    Sprite m_sprite;
};

}