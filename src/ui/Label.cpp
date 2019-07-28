#include <ui/Label.h>
#include <ui/UiManager.h>
#include <utils/Logging.h>

using namespace UI;

Label::Label()
{
    m_textNode = manager()->createElement<TextNode>( this );
    addChild( m_textNode );

    m_textNode->setBackgroundColour(Colour::Apricot);
}

void Label::setText(std::string const &text)
{
    m_textNode->setText( text );
}
