#pragma once

#include <queue>
#include <ui/Element.h>

namespace UI
{

class TextLog : public Element
{
public:
    explicit TextLog();
    ~TextLog() override = default;

    void addLine( std::string const& line );

    void scrollToPercent( float pos );

private:

    int m_maxLines;
    int m_maxVisibleLines;

    float m_scrollPosition;

    std::queue<std::string> m_lines;

};

}