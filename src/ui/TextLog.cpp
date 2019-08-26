#include <ui/TextLog.h>



UI::Internal::TextLogLayout::TextLogLayout(int height)
: m_height(height)
{

}


Vector2i UI::Internal::TextLogLayout::doLayout(UI::Element *ptr)
{
    auto tl = ptr->asType<TextLog>();

    for ( auto const& c: tl->children() )
    {

    }

    return {0, 0};
}


UI::TextLog::TextLog()
: m_maxLines(0), m_maxVisibleLines(0), m_scrollPosition(0.0f)
{
    setBorder( 2, Colour::Black );
    setBackgroundColour( Colour(200, 200, 200) );

    setLayout<Internal::TextLogLayout>( contentSize().y() );
}

void UI::TextLog::addLine(std::string const &line)
{
    m_lines.push(line);

    if ( m_lines.size() > m_maxLines )
    {
        m_lines.pop();
    }

    doLayout();
}