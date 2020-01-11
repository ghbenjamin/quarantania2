#include <ui/TextLog.h>
#include <ui/TextNode.h>
#include <ui/Manager.h>

UI::TextLog::TextLog()
: m_maxLines(10)
{
    setLayout<VerticalLayout>( 2, HAlignment::Left );
    setPadding(2);
    setBackgroundColour( Colour::Black );
}

void UI::TextLog::addLine(std::string const &line, Colour const& colour)
{
    onSizeSelf();

    // Is the content of the new line the same as the last line?
    if ( !m_lines.empty() && m_lines.back().text == line && m_lines.back().colour == colour )
    {
        // If so don't add a new line, just increment the count of our last line and rerender it
        m_lines.back().count++;
        children().back()->asType<TextNode>()->setText( m_lines.back().displayText() );
    }
    else
    {
        // Otherwise, put add a new line
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
