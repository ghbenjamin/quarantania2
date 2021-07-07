#pragma once

#include <ui/lib/Element.h>

namespace UI
{

class ScrollHolder : public Element
{
public:
    ScrollHolder(Manager* manager, Element* parent);
    ~ScrollHolder() override = default;

    float getScrollPercentage() const;
    void setScrollPercentage(float percentage );

    int getScrollHeight() const;

private:

    void onScroll( UMouseScrollEvent evt );

    int m_scrollHeight;
    float m_scrollPercentage;

};



}