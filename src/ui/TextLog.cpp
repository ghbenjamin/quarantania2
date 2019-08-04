#include <ui/TextLog.h>

UI::TextLog::TextLog()
: m_maxLines(0), m_maxVisibleLines(0), m_scrollPosition(0.0f)
{
    setBorder( 2, Colour::Black );
    setBackgroundColour( Colour(200, 200, 200) );
}

void UI::TextLog::addLine(std::string const &line)
{
    m_lines.push(line);

    if ( m_lines.size() > m_maxLines )
    {
        m_lines.pop();
    }
    else
    {
        doLayout();
    }
}

void UI::TextLog::scrollToPercent(float pos)
{
    m_scrollPosition = pos;
    doLayout();
}
