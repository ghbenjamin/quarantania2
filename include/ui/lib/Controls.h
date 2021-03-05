#pragma once

#include <ui/lib/Element.h>
#include <ui/lib/Defines.h>
#include <resource/Font.h>
#include <graphics/Texture.h>
#include <graphics/RenderObject.h>

namespace UI
{


class Label : public Element
{
public:
    Label(Manager* manager, Element* parent);
    Label(Manager* manager, Element* parent, TextStyle const& style);
    Label(Manager* manager, Element* parent, std::string const& label );
    ~Label() override = default;

    void setText( std::string const& text );
    void clearText();

    void setColour(Colour colour);

protected:
    void updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;

private:
    void onSizeSelf() override;
    void onMoveSelf() override;
    void renderText();

    Sprite m_text;
    TexturePtr m_rendered;
    RenderObject m_renderObject;
    std::string m_data;
    TextStyle m_style;
};




class Button : public Element
{
public:
    Button( Manager* manager, Element* parent );
    Button( Manager* manager, Element* parent, std::string const& text, std::function<void()> const& callback );
    ~Button() override = default;

    void setLabel( std::string const& label );
    void setCallback( std::function<void()> const& callback );

private:
    std::shared_ptr<Label> m_label;
    std::function<void()> m_callback;
};




class Icon : public Element
{
public:
    Icon(Manager* manager, Element* parent, Sprite const& img);
    Icon(Manager* manager, Element* parent, SpritesheetKey const& img);
    ~Icon() override = default;

    void setSprite( Sprite const& sprite );

protected:
    void updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;

private:
    Sprite m_sprite;
};

}