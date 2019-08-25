#include <ui/TextNode.h>
#include <resource/ResourceManager.h>
#include <graphics/RenderInterface.h>
#include <utils/Logging.h>

using namespace UI;

TextNode::TextNode(TextStyle const &style)
: m_style(style)
{
    m_font = ResourceManager::get().getResource<FontResource>( "fira-code-regular" )->get();
}

TextNode::TextNode() : TextNode({ Colour::Black })
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
    m_rendered = m_font->renderText( m_data, m_style.textColour );

    m_renderObject.texture = m_rendered->raw();
    m_renderObject.sourceRect = m_rendered->sourceRect();

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

