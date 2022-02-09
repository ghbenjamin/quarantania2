#include <ui/level/LevelMainMenu.h>
#include <ui/lib/Manager.h>
#include <resource/ResourceManager.h>
#include <game/Level.h>

using namespace UI;

LevelMainMenu::LevelMainMenu(Manager *manager, Element* parent, Level* level)
    : Element(manager, parent), m_level(level)
{
    manager->generateFromXML( "LevelMainMenu", this );
    
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border-solid" ).getPatch();
    setBackground( patch );
    setBorderWidth( patch.getBorderWidth() );

//    auto btnResume = manager->createElement<Button>( this, "Resume", [this](){ onBtnResume(); } );
//    auto btnOptions = manager->createElement<Button>( this, "Options", [this](){ onBtnResume(); } );
//    auto btnMainMenu = manager->createElement<Button>( this, "Main Menu", [this](){ onBtnExitMainMenu(); } );
//    auto btnQuit = manager->createElement<Button>( this, "Quit", [this](){ onBtnQuit(); } );
//
    
    
//    withId<Button>( "level-menu-resume" )->setCallback( [this](){ onBtnResume(); } );
//    withId( "level-menu-resume" );
    

    doLayout();
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
