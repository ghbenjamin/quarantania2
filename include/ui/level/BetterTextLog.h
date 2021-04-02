#pragma once

#include <ui/lib/Element.h>
#include "EndTurnButton.h"

namespace UI
{

class BetterTextLog;


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

private:
    static const int LineSpacing = 3;

    void layoutLines();

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
    
    float m_scrollPosition;
    std::shared_ptr<Icon> m_scrollbarTop;
    std::shared_ptr<Icon> m_scrollbarBottom;
    std::shared_ptr<BTLScrollBarLine> m_scrollbarLine;
    std::shared_ptr<Icon> m_scrollbarHandle;
};




class BetterTextLog : public Element
{
public:
    BetterTextLog(Manager* manager, Element* parent);
    ~BetterTextLog() override = default;

    void addLine( std::string const& data, Colour colour = Colour::White );
    void setScrollPosition( float pos );
    
private:
    float m_scrollPosition;
    std::shared_ptr<BTLScrollArea> m_scrollArea;
    std::shared_ptr<BTLScrollBar> m_scrollbar;

};



}