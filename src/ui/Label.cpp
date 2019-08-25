#include <ui/Label.h>
#include <ui/Manager.h>
#include <utils/Logging.h>

using namespace UI;

Label::Label()
{
    m_textNode = manager()->createElement<TextNode>(this);
}

void Label::setText(std::string const &text)
{
    m_textNode->setText( text );
}
