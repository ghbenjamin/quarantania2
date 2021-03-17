#pragma once

#include <queue>
#include <ui/lib/Element.h>
#include <resource/Font.h>
#include <graphics/Text.h>

namespace UI
{

//
//class TextLogLayout : ElementLayout
//{
//    Vector2i doLayout(Element *ptr) override;
//};
//
//
//struct TextLogItem
//{
//    std::string data;
//    Colour colour;
//    int count;
//};
//
//
//class Scrollbar : public Element
//{
//public:
//    Scrollbar(Manager* manager, Element* parent);
//    ~Scrollbar() override = default;
//
//protected:
//    void onSizeSelf() override;
//
//private:
//    std::shared_ptr<Element> m_scrollThumb;
//};
//
//class TextLogTextArea : public Element
//{
//public:
//    TextLogTextArea(Manager* manager, Element* parent);
//    ~TextLogTextArea() override = default;
//};
//
//class MainTextLog : public Element
//{
//public:
//    MainTextLog(Manager* manager, Element* parent);
//    ~MainTextLog() override = default;
//
//    void addLine(std::string const& line, Colour const& colour = Colour::White );
//
//private:
//    std::queue<TextLogItem> m_lines;
//    int m_maxLines;
//
//    std::shared_ptr<Scrollbar> m_scrollBar;
//    std::shared_ptr<TextLogTextArea> m_textarea;
//};
//
//



















class MainTextLog;


class TextLogLineData
{
public:
    friend class MainTextLog;

    TextLogLineData(const std::string &text, int count, const Colour &colour);
    ~TextLogLineData() = default;
    
    TextRenderObj const& sprite() const;
    void setPos( Vector2i pos );
    void renderText(std::shared_ptr<FtFontFace>& font, RectI bounds);

    RenderObject getRenderObj();

private:
    // Generate the text for this line, including the (count x2)
    std::string displayText() const;

    std::string m_text;
    int m_count;
    Colour m_colour;

    Vector2i m_pos;
    TextRenderObj m_sprite;
};



class MainTextLog : public Element
{
public:
    MainTextLog(Manager* manager, Element* parent);
    ~MainTextLog() override = default;

    void addLine(std::string_view line, Colour const& colour = Colour::White );

private:
    void updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter) override;
    void onMoveSelf() override;
    void onSizeSelf() override;

    void layoutLines();

    static constexpr int LineSpacing = 2;
    std::shared_ptr<FtFontFace> m_font;
    int m_maxLines;
    std::list<TextLogLineData> m_lines;
    int m_scrollPos;
};


}