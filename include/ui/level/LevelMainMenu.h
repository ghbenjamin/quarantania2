#pragma once

#include <ui/lib/Element.h>

namespace UI
{

class LevelMainMenu : public Element
{
public:

    LevelMainMenu(Manager* manager, Element* parent);
    ~LevelMainMenu() override = default;

private:

    void onBtnResume();
    void onBtnExitMainMenu();
    void onBtnQuit();
    void onBtnOptions();

};

}