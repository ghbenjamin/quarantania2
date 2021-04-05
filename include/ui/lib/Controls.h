#pragma once

#include <ui/lib/Element.h>
#include <ui/lib/Defines.h>
#include <graphics/Texture.h>
#include <graphics/RenderObject.h>
#include <graphics/Text.h>

namespace UI
{


class Label : public Element
{
public:
    Label(Manager* manager, Element* parent);
    Label(Manager* manager, Element* parent, TextStyle const& style);
    Label(Manager* manager, Element* parent, std::string const& label );
    Label(Manager* manager, Element* parent, std::string const& label, Colour colour, int size );
    ~Label() override = default;

    void setText( std::string const& text );
    void setTextColour( Colour colour );
    void setTextSize( int size );

protected:
    void updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;
    void onAlphaModChange(float newValue) override;
    void onSizeSelf() override;

private:
    void renderText();

    std::optional<TextRenderObj> m_text;
    std::string m_data;
    TextStyle m_style;
    bool m_noRerender;
};




class Button : public Element
{
public:
    Button( Manager* manager, Element* parent, std::string const& text, std::function<void()> const& callback );
    ~Button() override = default;

    void setLabel( std::string const& label );
    void setCallback( std::function<void()> const& callback );

private:

    Colour m_mouseOverColour = Colour::Grey;
    Colour m_defaultColour = Colour{200, 200, 200, 255};

    std::shared_ptr<Label> m_label;
    std::function<void()> m_callback;
};


class Icon : public Element
{
public:
    Icon(Manager* manager, Element* parent);
    Icon(Manager* manager, Element* parent, Sprite const& img);
    Icon(Manager* manager, Element* parent, SpritesheetKey const& img);
    Icon(Manager* manager, Element* parent, std::shared_ptr<Texture> const& img);
    ~Icon() override = default;

    void setSprite( Sprite const& sprite );
    void setSprite( SpritesheetKey const& sprite );
    void clearSprite();
    
    bool hasSprite() const;
    Sprite& getSprite();
    
protected:
    void updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;
    void onAlphaModChange(float newValue) override;
    
    std::optional<Sprite> m_sprite;
};


class IconButton : public Element
{
public:
    IconButton( Manager* manager, Element* parent, SpritesheetKey icon, std::function<void()> const& callback );
    ~IconButton() override = default;
    
    void setIcon( SpritesheetKey icon );
    void setCallback( std::function<void()> const& callback );

private:
    
    Colour m_mouseOverColour = Colour::Grey;
    Colour m_defaultColour = Colour::White;
    
    std::shared_ptr<Icon> m_icon;
    std::function<void()> m_callback;
};


class Spacer : public Element
{
public:
    Spacer( Manager* manager, Element* parent, Vector2i size );
    ~Spacer() override = default;
};

class HorizontalRule : public Element
{
public:
    HorizontalRule( Manager* manager, Element* parent, Colour colour );
    ~HorizontalRule() override = default;
};

}