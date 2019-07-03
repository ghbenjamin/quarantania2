#include <ui/TextNode.h>
#include <resource/ResourceManager.h>
#include <graphics/RenderInterface.h>
#include <utils/Logging.h>

using namespace UI;

TextNode::TextNode(TextStyle const &style)
: m_style(style)
{
    m_font = ResourceManager::get().getResource<FontResource>( "roboto-regular" )->get();
}

TextNode::TextNode() : TextNode({ Colour::Green })
{

}


void TextNode::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    rInter.addScreenItem(m_renderObject);
}

void TextNode::renderText()
{
    m_rendered = m_font->renderText( m_data, m_style.textColour );

    m_renderObject.texture = m_rendered->raw();
    m_renderObject.sourceRect = m_rendered->sourceRect();
    m_renderObject.targetRect.w = m_rendered->sourceRect().w;
    m_renderObject.targetRect.h = m_rendered->sourceRect().h;
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

