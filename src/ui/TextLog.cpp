#include <ui/TextLog.h>

UI::TextLog::TextLog()
: m_maxLines(0), m_maxVisibleLines(0)
{
    setBorder( 2, Colour::Black );
    setBackgroundColour( Colour(200, 200, 200) );
}

void UI::TextLog::addLine(std::string const &line)
{

}
