#pragma once

#include <ui/lib/Element.h>

namespace UI
{

class BottomLeftBar : public Element
{
public:
    BottomLeftBar(Manager* manager, Element* parent);
    ~BottomLeftBar() override = default;
};


class IconButton;

class BottomMenubar : public Element
{
public:
    BottomMenubar(Manager* manager, Element* parent);
    ~BottomMenubar() override = default;

private:
    
    void onBtnEndTurn();
    void onBtnJournal();
    void onBtnMap();
    void onBtnSettings();
    
    std::shared_ptr<IconButton> m_btnEndTurn;
    std::shared_ptr<IconButton> m_btnJournal;
    std::shared_ptr<IconButton> m_btnMap;
    std::shared_ptr<IconButton> m_btnSettings;
    
};


}