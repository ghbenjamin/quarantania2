#include <ui/level/Composites.h>
#include <ui/lib/Manager.h>

#include <ui/level/ActionPopupMenu.h>
#include <resource/ResourceManager.h>
#include <game/Level.h>


using namespace UI;

UI::BottomLeftBar::BottomLeftBar(UI::Manager *manager, UI::Element *parent)
        : Element(manager, parent)
{
    setLayout<HorizontalLayout>( 4, VAlignment::Centre );

    auto actionMenu = manager->createElement<UI::ActionMenu>(this);
}

BottomMenubar::BottomMenubar( Manager *manager, Element *parent ) : Element(manager, parent)
{
    setLayout<HorizontalLayout>( 4, VAlignment::Centre );
    
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border-solid" ).getPatch();
    setBackground( patch );
    setBorderWidth( patch.getBorderWidth() );
    
    m_btnEndTurn = manager->createElement<IconButton>( this, "game_ui/w-hourglass", [this](){ onBtnEndTurn(); } );
    m_btnSettings = manager->createElement<IconButton>( this, "game_ui/w-gears", [this](){ onBtnSettings(); } );
}

void BottomMenubar::onBtnEndTurn()
{
    manager()->level()->events().broadcast<GameEvents::TurnChange>(false);
}

void BottomMenubar::onBtnSettings()
{

}
