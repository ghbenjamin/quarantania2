#include <ui/TextLog.h>
#include <ui/TextNode.h>
#include <ui/Manager.h>

UI::TextLog::TextLog()
: m_maxLines(10)
{
    setLayout<VerticalLayout>( 2, HAlignment::Left );
    setPadding(2);
    setBackgroundColour( Colour::Black );
    setBorder(2, Colour::White);
}

void UI::TextLog::addLine(std::string const &line, Colour const& colour)
{
    onSizeSelf();

    if ( !m_lines.empty() &&  m_lines.back().text == line && m_lines.back().colour == colour )
    {
        m_lines.back().count++;
        children().back()->asType<TextNode>()->setText( m_lines.back().displayText() );
    }
    else
    {
        m_lines.push_back({ line, 1, colour });

        auto tnode = manager()->createElement<TextNode>(this, TextStyle{ m_lines.back().colour });
        tnode->setMaximumOuterSize( contentSize() );
        tnode->setText( m_lines.back().displayText() );

        if ( m_lines.size() > m_maxLines )
        {
            m_lines.pop_front();
            removeChild( children().front() );
        }
    }

    doLayout();
}

void UI::TextLog::onSizeSelf()
{
    auto size = bounds().right();
    for ( auto& child : children() )
    {
        child->setMaximumOuterSize( size );
    }
}

std::string UI::TextLogLineData::displayText() const
{
    if ( count > 1 )
    {
        return fmt::format( "{} (x{})", this->text, this->count );
    }
    else
    {
        return this->text;
    }
}

Vector2i UI::TextLogLayout::doLayout(UI::Element *ptr)
{
    return {0, 0};
}
