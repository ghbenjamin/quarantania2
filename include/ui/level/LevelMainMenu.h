#pragma once

#include <ui/lib/Element.h>

class Level;

namespace UI
{

class LevelMainMenu : public Element
{
public:

    LevelMainMenu(Manager* manager, Element* parent, Level* level);
    ~LevelMainMenu() override = default;

private:

    void onBtnResume();
    void onBtnExitMainMenu();
    void onBtnQuit();
    void onBtnOptions();

    Level* m_level;

};

}