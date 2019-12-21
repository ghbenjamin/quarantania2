#pragma once

#include <queue>
#include <ui/Element.h>

namespace UI
{

namespace Internal
{
class TextLogLayout : public ElementLayout
{
public:
    explicit TextLogLayout(int height);
    ~TextLogLayout() override = default;
    Vector2i doLayout(Element *ptr) override;

private:
    int m_height;
};
}


class TextLog : public Element
{
public:
    explicit TextLog();
    ~TextLog() override = default;

    void addLine( std::string const& line );

private:

    int m_maxLines;
    int m_maxVisibleLines;

    float m_scrollPosition;

    std::queue<std::string> m_lines;


};

}