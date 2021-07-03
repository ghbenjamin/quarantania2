#include <ui/level/Composites.h>
#include <ui/lib/Manager.h>

#include <ui/level/ActionPopupMenu.h>
#include <resource/ResourceManager.h>
#include <game/Level.h>
#include <ui/level/LevelMainMenu.h>


using namespace UI;

UI::BottomLeftBar::BottomLeftBar(UI::Manager *manager, UI::Element *parent, Level* level)
    : Element(manager, parent)
{
    setLayout<HorizontalLayout>( 4, VAlignment::Centre );

    auto actionMenu = manager->createElement<UI::ActionMenu>(this, level);
}

BottomMenubar::BottomMenubar( Manager *manager, UI::Element* parent, Level* level )
    : Element(manager, parent), m_level(level)
{
    setLayout<HorizontalLayout>( 4, VAlignment::Centre );
    
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border-solid" ).getPatch();
    setBackground( patch );
    setBorderWidth( patch.getBorderWidth() );
    
    m_btnEndTurn = manager->createElement<IconButton>( this, "game_ui/w-hourglass", [this](){ onBtnEndTurn(); } );
    m_btnJournal = manager->createElement<IconButton>( this, "game_ui/w-scroll-unfurled", [this](){ onBtnJournal(); } );
    m_btnMap = manager->createElement<IconButton>( this, "game_ui/w-compass", [this](){ onBtnMap(); } );
    m_btnSettings = manager->createElement<IconButton>( this, "game_ui/w-gears", [this](){ onBtnSettings(); } );
}

void BottomMenubar::onBtnEndTurn()
{
    m_level->events().broadcast<GameEvents::TurnChange>(false);
}

void BottomMenubar::onBtnSettings()
{
    auto menu = manager()->createElement<LevelMainMenu>(nullptr, m_level);
    manager()->makeElementModal(menu);
    manager()->centreElementInWindow(menu);
}

void BottomMenubar::onBtnJournal()
{

}

void BottomMenubar::onBtnMap()
{

}
