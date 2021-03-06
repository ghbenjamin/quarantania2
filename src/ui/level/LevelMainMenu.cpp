#include <ui/level/LevelMainMenu.h>
#include <ui/lib/Manager.h>
#include <resource/ResourceManager.h>
#include <game/Level.h>

using namespace UI;

LevelMainMenu::LevelMainMenu(Manager *manager, Element* parent, Level* level)
    : Element(manager, parent), m_level(level)
{
    setLayout<VerticalLayout>(4, HAlignment::Fill );
    setPadding(8);

    addHotkey( SDLK_ESCAPE, [this](){
        onBtnResume();
    });

    auto const& patch = ResourceManager::get().getNinePatch( "simple-border-solid" ).getPatch();
    setBackground( patch );
    setBorderWidth( patch.getBorderWidth() );

    auto btnResume = manager->createElement<Button>( this, "Resume", [this](){ onBtnResume(); } );
    auto btnOptions = manager->createElement<Button>( this, "Options", [this](){ onBtnResume(); } );
    auto btnMainMenu = manager->createElement<Button>( this, "Main Menu", [this](){ onBtnExitMainMenu(); } );
    auto btnQuit = manager->createElement<Button>( this, "Quit", [this](){ onBtnQuit(); } );

    auto buttons = {btnResume, btnOptions, btnMainMenu, btnQuit};

    for (auto& b : buttons)
    {
        b->setPreferredContentSize({100, 25});
        b->getLabel().setTextSize(18);
        b->setLayout<CenterLayout>();
    }

    doLayout();
}

void LevelMainMenu::onBtnResume()
{
    deleteSelf();
}

void LevelMainMenu::onBtnExitMainMenu()
{
    m_level->setLevelExitStatus( LevelExitStatus::MainMenu );
}

void LevelMainMenu::onBtnQuit()
{
    m_level->setLevelExitStatus( LevelExitStatus::Desktop );
}

void LevelMainMenu::onBtnOptions()
{

}
