#pragma once

#include <queue>
#include <ui/Element.h>
#include <resource/Font.h>

namespace UI
{

class TextLog;


class TextLogLineData
{
public:
    friend class TextLog;

    TextLogLineData(const std::string &text, int count, const Colour &colour);
    ~TextLogLineData() = default;

    Sprite const& sprite() const;
    void setPos( Vector2i pos );
    void renderText(FontPtr& font, RectI bounds);

    [[nodiscard]] RenderObject getRenderObj() const;

private:
    // Generate the text for this line, including the (count x2)
    [[nodiscard]] std::string displayText() const;

    std::string m_text;
    int m_count;
    Colour m_colour;

    Vector2i m_pos;
    Sprite m_sprite;
};



class TextLog : public Element
{
public:
    explicit TextLog();
    ~TextLog() override = default;

    void addLine( std::string const& line, Colour const& colour = Colour::White );

private:
    void updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;
    void onMoveSelf() override;
    void onSizeSelf() override;

    void layoutLines();

    static constexpr int LineSpacing = 2;
    FontPtr m_font;
    int m_maxLines;
    std::list<TextLogLineData> m_lines;
    int m_scrollPos;
};




}