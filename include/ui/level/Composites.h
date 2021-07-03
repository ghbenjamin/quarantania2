#pragma once

#include <ui/lib/Element.h>

class Level;

namespace UI
{

class BottomLeftBar : public Element
{
public:
    BottomLeftBar(Manager* manager, Element* parent, Level* level);
    ~BottomLeftBar() override = default;
};


class IconButton;

class BottomMenubar : public Element
{
public:
    BottomMenubar(Manager* manager, Element* parent, Level* level);
    ~BottomMenubar() override = default;

private:
    
    void onBtnEndTurn();
    void onBtnJournal();
    void onBtnMap();
    void onBtnSettings();
    
    Level* m_level;
    std::shared_ptr<IconButton> m_btnEndTurn;
    std::shared_ptr<IconButton> m_btnJournal;
    std::shared_ptr<IconButton> m_btnMap;
    std::shared_ptr<IconButton> m_btnSettings;
    
};


}