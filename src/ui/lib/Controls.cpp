#include <ui/lib/Controls.h>
#include <resource/ResourceManager.h>
#include <graphics/RenderInterface.h>
#include <utils/Logging.h>
#include <ui/lib/Manager.h>

using namespace UI;

// Text node
// ---------------------------------------


Label::Label(Manager* manager, Element* parent, TextStyle const &style)
        : Element(manager, parent), m_style(style)
{
}

Label::Label(Manager* manager, Element* parent)
        : Label(manager, parent, {Colour::Black, ResourceManager::get().getDefaultFont(14)})
{
}

Label::Label(Manager *manager, Element *parent, const std::string &label)
        : Label(manager, parent)
{
    setText(label);
}


void Label::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    if ( m_text )
    {
        auto pos = globalPosition() + contentOffset();
        rInter.addScreenItem(m_text.renderObject( pos ));
    }
}

void Label::renderText()
{
    if ( m_data.empty() )
        return;

    if ( hasMaximumOuterSize() )
    {
        m_text = { m_style.font->renderText( m_data, m_style.textColour, maxOuterSize().x() ) };
    }
    else
    {
        m_text = { m_style.font->renderText( m_data, m_style.textColour ) };
    }
    
    m_text.setRenderLayer(RenderLayer::UI);
    onMoveSelf();
}

void Label::setText(std::string const &text)
{
    m_data = text;
    renderText();

    setPreferredContentSize( m_text.size() );
}

void Label::setTextColour( Colour colour)
{
    if (colour != m_style.textColour)
    {
        m_style.textColour = colour;
        renderText();
    }
}

void Label::onColourModChange()
{
    m_text.setColour( getColour() );
}




// Button
// ---------------------------------------

UI::Button::Button(Manager *manager, Element *parent, const std::string &text, std::function<void()> const& callback)
        : Element(manager, parent)
{
    setPadding( 4 );
    setBackground( Colour{200, 200, 200, 255} );

    m_label = manager->createElement<UI::Label>(this );
    m_label->setText( text );

    setCallback( callback );
}

Button::Button(Manager *manager, Element *parent)
        : Button(manager, parent, "", {} ) {}

void Button::setLabel(const std::string &label)
{
    m_label->setText(label);
}

void Button::setCallback(const std::function<void()> &callback)
{
    if (callback)
    {
        if (m_callback)
        {
            removeEventCallbacks( UEventType::Click );
        }

        m_callback = callback;

        addEventCallback(UEventType::Click, [this] (UEvent& evt) {
            this->m_callback();
        });
    }
}


// Icon
// ---------------------------------------

Icon::Icon(Manager *manager, Element *parent, const Sprite &img)
        : Element(manager, parent), m_sprite(img)
{
    m_sprite->setRenderLayer(RenderLayer::UI);
    setPreferredContentSize(m_sprite->size());
}

Icon::Icon( Manager *manager, Element *parent )
    : Element(manager, parent), m_sprite {} {}
    
Icon::Icon(Manager *manager, Element *parent, SpritesheetKey const& img)
    : Icon(manager, parent, ResourceManager::get().getSprite(img)) {}
    
void Icon::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    if ( m_sprite.has_value() )
    {
        auto pos = globalPosition() + contentOffset();
        rInter.addScreenItem( m_sprite->renderObject(pos) );
    }
}

void Icon::setSprite( Sprite const& sprite )
{
    m_sprite = sprite;
    m_sprite->setRenderLayer( RenderLayer::UI );
    setPreferredContentSize( m_sprite->size() );
}

void Icon::setSprite( SpritesheetKey const &sprite )
{
    setSprite( ResourceManager::get().getSprite(sprite) );
}

void Icon::onColourModChange()
{
    m_sprite->setColour( getColour() );
}

void Icon::clearSprite()
{
    m_sprite.reset();
}



