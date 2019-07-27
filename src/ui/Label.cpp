#include <ui/Label.h>
#include <ui/UiManager.h>

using namespace UI;

Label::Label()
{
    m_textNode = manager()->createElement<TextNode>( this );
    addChild( m_textNode );
}

void Label::setText(std::string const &text)
{
    m_textNode->setText( text );
}
