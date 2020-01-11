#pragma once

#include <queue>
#include <ui/Element.h>

namespace UI
{


class TextLogLayout : public ElementLayout
{
public:
    TextLogLayout() = default;
    ~TextLogLayout() override = default;

    Vector2i doLayout(Element *ptr) override;
};

struct TextLogLineData
{
    std::string text;
    int count;
    Colour colour;

    // Generate the text for this line, including the (count x2)
    std::string displayText() const;
};

class TextLog : public Element
{
public:
    explicit TextLog();
    ~TextLog() override = default;

    void addLine( std::string const& line, Colour const& colour = Colour::White );

private:

    void onSizeSelf() override;

    int m_maxLines;
    std::list<TextLogLineData> m_lines;
};

}