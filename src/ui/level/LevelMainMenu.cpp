#include <ui/level/LevelMainMenu.h>
#include <ui/lib/Manager.h>
#include <game/Level.h>

using namespace UI;

LevelMainMenu::LevelMainMenu(Manager *manager, Element* parent, Level* level)
    : Element(manager, parent), m_level(level)
{
    manager->generateFromLua( "ui/elements/LevelMainMenu", this );
    
    withId<Button>( "level-menu-resume" )->setCallback( [this](){ onBtnResume(); } );
    withId<Button>( "level-menu-options" )->setCallback( [this](){ onBtnResume(); } );
    withId<Button>( "level-menu-main" )->setCallback( [this](){ onBtnExitMainMenu(); } );
    withId<Button>( "level-menu-quit" )->setCallback( [this](){ onBtnQuit(); } );
}

void LevelMainMenu::onBtnResume()
{
    deleteSelf();
}

void LevelMainMenu::onBtnExitMainMenu()
{
    m_level->setExitStatus(LevelExitStatus::MainMenu);
}

void LevelMainMenu::onBtnQuit()
{
    m_level->setExitStatus(LevelExitStatus::Desktop);
}

void LevelMainMenu::onBtnOptions()
{

}
