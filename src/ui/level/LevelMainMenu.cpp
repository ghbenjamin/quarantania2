#include <ui/level/LevelMainMenu.h>
#include <ui/lib/Manager.h>
#include <resource/ResourceManager.h>

using namespace UI;

LevelMainMenu::LevelMainMenu(Manager *manager, Element* parent)
    : Element(manager, parent)
{
    setLayout<VerticalLayout>(4, HAlignment::Fill );
    setPadding(8);

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
        b->setPreferredContentSize({40, 20});
        b->getLabel().setTextSize(18);
    }

    doLayout();
}

void LevelMainMenu::onBtnResume()
{
    manager()->deleteElement( shared_from_this() );
}

void LevelMainMenu::onBtnExitMainMenu()
{

}

void LevelMainMenu::onBtnQuit()
{

}

void LevelMainMenu::onBtnOptions()
{

}
