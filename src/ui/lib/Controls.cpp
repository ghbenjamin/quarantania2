#include <ui/lib/Controls.h>
#include <resource/ResourceManager.h>
#include <graphics/RenderInterface.h>
#include <utils/Logging.h>
#include <ui/lib/Manager.h>

using namespace UI;

// Text node
// ---------------------------------------


Label::Label(Manager* manager, Element* parent, TextStyle const &style)
        : Element(manager, parent), m_style(style), m_noRerender(false) {}

Label::Label(Manager* manager, Element* parent)
        : Label(manager, parent, {Colour::Black, ResourceManager::get().getDefaultFont(), 14}) {}

Label::Label( Manager *manager, Element *parent, TextStyle const &style, std::string const &label )
    : Label(manager, parent, style)
{
    setText(label);
}

Label::Label(Manager *manager, Element *parent, const std::string &label)
        : Label(manager, parent)
{
    setText(label);
}


Label::Label( Manager *manager, Element *parent, std::string const &label, Colour colour, int size )
    : Label(manager, parent, TextStyle{ colour, ResourceManager::get().getDefaultFont(), size })
{
    setText(label);
}

void Label::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    if ( m_text )
    {
        auto pos = globalPosition() + contentOffset();
        rInter.addItem(m_text->renderObject( pos ), RenderLayer::UI);
    }
}

void Label::renderText()
{
    if ( m_data.empty() )
        return;
        
    if ( m_noRerender )
    {
        return;
    }

    if ( hasMaximumOuterSize() )
    {
        m_text = { m_style.font->renderString( m_data, m_style.fontSize, m_style.textColour, maxOuterSize().x() ) };
    }
    else
    {
        m_text = { m_style.font->renderString( m_data, m_style.fontSize, m_style.textColour ) };
    }

    // Guard with noRerender to break a loop
    m_noRerender = true;
    setPreferredContentSize( m_text->getSize() );
    m_noRerender = false;
}

void Label::setText(std::string const &text)
{
    m_data = text;
    renderText();
}

void Label::setTextColour( Colour colour )
{
    if (colour != m_style.textColour)
    {
        m_style.textColour = colour;
        renderText();
    }
}

void Label::onAlphaModChange(float newValue)
{
    if (m_text)
        m_text->setColour( m_style.textColour.withAlphaF(newValue) );
}

void Label::setTextSize(int size)
{
    m_style.fontSize = size;
    renderText();
}

void Label::onSizeSelf()
{
    renderText();
}



// Button
// ---------------------------------------

UI::Button::Button(Manager *manager, Element *parent, const std::string &text, std::function<void()> const& callback)
        : Element(manager, parent)
{
    setPadding( 4 );
    setBackground( m_defaultColour );

    m_label = manager->createElement<UI::Label>(this );
    m_label->setText( text );

    setCallback( callback );
    
    addEventCallback( UEventType::MouseIn, [this](UEvent const& evt) {
        getBackground().setColourMod( m_mouseOverColour );
    });
    
    addEventCallback( UEventType::MouseOut, [this](UEvent const& evt) {
        getBackground().setColourMod( m_defaultColour );
    });
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

Label &Button::getLabel()
{
    return *m_label;
}


// Icon
// ---------------------------------------

Icon::Icon(Manager *manager, Element *parent, const Sprite &img)
        : Element(manager, parent), m_sprite(img)
{
    setPreferredContentSize(m_sprite->size());
}

Icon::Icon( Manager *manager, Element *parent )
    : Element(manager, parent), m_sprite {} {}
    
Icon::Icon(Manager *manager, Element *parent, SpritesheetKey const& img)
    : Icon(manager, parent, ResourceManager::get().getSprite(img)) {}


Icon::Icon( Manager *manager, Element *parent, std::shared_ptr<Texture> const &img )
    : Icon( manager, parent, Sprite{img} ) {}



void Icon::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    if ( m_sprite.has_value() )
    {
        auto pos = globalPosition() + contentOffset();
        rInter.addItem( m_sprite->renderObject(pos), RenderLayer::UI );
    }
}

void Icon::setSprite( Sprite const& sprite )
{
    m_sprite = sprite;
    setPreferredContentSize( m_sprite->size() );
}

void Icon::setSprite( SpritesheetKey const &sprite )
{
    setSprite( ResourceManager::get().getSprite(sprite) );
}

void Icon::onAlphaModChange(float newValue)
{
    if (m_sprite)
    {
        m_sprite->setAlphaMod(newValue);
    }
    
}

void Icon::clearSprite()
{
    m_sprite.reset();
}

bool Icon::hasSprite() const
{
    return m_sprite.has_value();
}

Sprite &Icon::getSprite()
{
    return *m_sprite;
}


// Icon button
// ---------------------------------------

IconButton::IconButton( Manager *manager, Element *parent, SpritesheetKey icon, std::function<void()> const &callback )
    : Element(manager, parent), m_callback(callback), m_isDisabled(false)
{
    m_icon = manager->createElement<Icon>(this, icon);
    
    setPadding(2);
    setCallback( callback );
    
    addEventCallback( UEventType::MouseIn, [this](UEvent const& evt) {
        if (!m_isDisabled)
        {
            m_icon->getSprite().setColourMod( m_mouseOverColour );
            ResourceManager::get().getWindow()->cursor().setCursorType( CursorType::Interact );
        }
    });
    
    addEventCallback( UEventType::MouseOut, [this](UEvent const& evt) {
        if (!m_isDisabled)
        {
            m_icon->getSprite().resetColourMod();
            ResourceManager::get().getWindow()->cursor().resetCursor();
        }
    });
}


void IconButton::setIcon( SpritesheetKey icon )
{
    m_icon->setSprite( icon );
    doLayout();
}

void IconButton::setCallback( std::function<void()> const &callback )
{
    if (callback)
    {
        if (m_callback)
        {
            removeEventCallbacks( UEventType::Click );
        }
        
        m_callback = callback;
        
        addEventCallback(UEventType::Click, [this] (UEvent& evt) {
            if (!m_isDisabled)
            {
                this->m_callback();
            }
        });
    }
}

void IconButton::setDisabled( bool val )
{
    m_isDisabled = val;
    
    if (val)
    {
        m_icon->setAlphaMod(0.5f);
    }
    else
    {
        m_icon->setAlphaMod(1.0f);
    }
}

bool IconButton::isDisabled() const
{
    return m_isDisabled;
}

std::shared_ptr<Icon> IconButton::getIcon()
{
    return m_icon;
}



// Spacer
// ---------------------------------------

Spacer::Spacer(Manager *manager, Element *parent, Vector2i size)
    : Element(manager, parent)
{
    setPreferredContentSize(size);
}


// Horizontal rule
// ---------------------------------------

HorizontalRule::HorizontalRule(Manager *manager, Element *parent, Colour colour)
    : Element(manager, parent)
{
    setBackground(colour);
    setPreferredContentSize({20, 1});
}
