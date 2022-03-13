#pragma once

#include <ui/lib/Element.h>
#include <ui/lib/Controls.h>

class Level;

namespace UI
{

class EventLog;

struct BTLLineData
{
    BTLLineData( std::string const& data, Colour colour );
    ~BTLLineData() = default;
    
    std::string data;
    int count;
    Colour colour;
};


class BTLScrollArea : public Element
{
public:
    BTLScrollArea(Manager* manager, Element* parent);
    ~BTLScrollArea() override = default;
    
    void addLine( std::string const& data, Colour colour = Colour::White );
    void setScrollPosition(float position);

private:
    static const int LineSpacing = 3;
    
    void layoutLines();
    
    Level* m_level;
    float m_scrollPosition;
    std::list<std::shared_ptr<Label>> m_lines;
};


class BTLScrollBarLine : public Icon
{
public:
    using Icon::Icon;

protected:
    void onSizeSelf() override;
};


class BTLScrollBar : public Element
{
public:
    BTLScrollBar(Manager* manager, Element* parent);
    ~BTLScrollBar() override = default;
    
    void setScrollPosition( float scrollPos );

protected:
    void onSizeSelf() override;
    void onMoveSelf() override;

private:
    void layoutElements();
    void placeScrollHandle();
    void onClick( Vector2i pos );
    
    static constexpr float ScrollPerTopBottomClick = 0.2f;
    
    Level* m_level;
    float m_scrollPosition;
    std::shared_ptr<Icon> m_scrollbarTop;
    std::shared_ptr<Icon> m_scrollbarBottom;
    std::shared_ptr<BTLScrollBarLine> m_scrollbarLine;
    std::shared_ptr<Icon> m_scrollbarHandle;
};


class TextLog : public Element
{
public:
    TextLog( Manager* manager, Element* parent);
    ~TextLog() override = default;
    
    void addLine( std::string const& data, Colour colour = Colour::White );
    void setScrollPosition( float pos );
    void moveScrollPosition( float dy );

protected:
    void onSizeSelf() override;

private:
    float m_scrollPosition;
    std::shared_ptr<BTLScrollArea> m_scrollArea;
    std::shared_ptr<BTLScrollBar> m_scrollbar;
    
};

}