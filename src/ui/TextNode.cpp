#include <ui/TextNode.h>
#include <resource/ResourceManager.h>

using namespace UI;

TextNode::TextNode(TextStyle const &style)
: m_style(style)
{
    m_font = ResourceManager::get().getResource<FontResource>( "roboto" )->get();
}

TextNode::TextNode() : TextNode({ Colour::Black })
{

}


void TextNode::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{

}

void TextNode::renderText()
{
    m_rendered = m_font->renderText( m_data, m_style.textColour );
    m_renderObject.texture = m_rendered->raw();
}

void TextNode::setText(std::string const &text)
{
    m_data = text;
    renderText();
}

void TextNode::onSizeSelf()
{

}

void TextNode::onMoveSelf()
{

}

