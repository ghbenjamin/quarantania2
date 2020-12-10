#include <ui/Controls.h>
#include <resource/ResourceManager.h>
#include <graphics/RenderInterface.h>
#include <utils/Logging.h>
#include <ui/Manager.h>

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


void Label::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    if ( m_renderObject.texture != nullptr )
    {
        rInter.addScreenItem(m_renderObject);
    }
}

void Label::renderText()
{
    if ( m_data.empty() )
        return;

    if ( hasMaximumOuterSize() )
    {
        m_rendered = m_style.font->renderText( m_data, m_style.textColour, maxOuterSize().x() );
    }
    else
    {
        m_rendered = m_style.font->renderText( m_data, m_style.textColour );
    }

    m_renderObject.texture = m_rendered->raw();
    m_renderObject.sourceRect = m_rendered->sourceRect();
    m_renderObject.renderLayer = RenderLayer::UI;
    m_renderObject.targetRect = {
            0,
            0,
            m_rendered->sourceRect().w,
            m_rendered->sourceRect().h
    };

    onMoveSelf();
}

void Label::setText(std::string const &text)
{
    m_data = text;
    renderText();

    setPreferredContentSize( m_rendered->size() );
}

void Label::onSizeSelf()
{

}

void Label::onMoveSelf()
{
    auto pos = globalPosition() + contentOffset();
    m_renderObject.targetRect.x = pos.x();
    m_renderObject.targetRect.y = pos.y();
}

void Label::clearText()
{
    m_renderObject = RenderObject();
    m_data = "";
    setPreferredContentSize({0, 0});
}

void Label::setColour(Colour colour)
{
    if (colour != m_style.textColour)
    {
        m_style.textColour = colour;
        renderText();
    }
}



// Button
// ---------------------------------------

UI::Button::Button(Manager *manager, Element *parent, const std::string &text, std::function<void()> const& callback)
        : Element(manager, parent), m_callback(callback)
{
    setPadding( 4 );
    setBackgroundColour({200, 200, 200, 255});

    auto textNode = manager->createElement<UI::Label>(this );
    textNode->setText( text );

    addEventCallback(UEventType::Click, [this] (UEvent& evt) {
        this->m_callback();
    });
}

// Button
// ---------------------------------------


Icon::Icon(Manager *manager, Element *parent, const Sprite &img)
        : Element(manager, parent), m_sprite(img)
{
    m_sprite.setRenderLayer(RenderLayer::UI);
    setPreferredContentSize(m_sprite.size());
}

Icon::Icon(Manager *manager, Element *parent, SpritesheetKey const& img)
    : Icon(manager, parent, ResourceManager::get().getSprite(img)) {}


void Icon::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    auto pos = globalPosition() + contentOffset();
    rInter.addScreenItem( m_sprite.renderObject(pos) );
}

