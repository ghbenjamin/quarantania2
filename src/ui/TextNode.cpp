#include <ui/TextNode.h>
#include <resource/ResourceManager.h>
#include <graphics/RenderInterface.h>
#include <utils/Logging.h>

using namespace UI;

TextNode::TextNode(Manager* manager, Element* parent, TextStyle const &style)
: Element(manager, parent), m_style(style)
{
}

TextNode::TextNode(Manager* manager, Element* parent)
    : TextNode(manager, parent, { Colour::Black, ResourceManager::get().getFont( "inconsolata-regular", 14 )})
{
}


void TextNode::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    if ( m_renderObject.texture != nullptr )
    {
        rInter.addScreenItem(m_renderObject);
    }
}

void TextNode::renderText()
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

void TextNode::setText(std::string const &text)
{
    m_data = text;
    renderText();

    setPreferredContentSize( m_rendered->size() );
}

void TextNode::onSizeSelf()
{

}

void TextNode::onMoveSelf()
{
    auto pos = globalPosition();
    m_renderObject.targetRect.x = pos.x();
    m_renderObject.targetRect.y = pos.y();
}

void TextNode::clearText()
{
    m_renderObject = RenderObject();
    m_data = "";

    // TODO Also update preferred size
}

