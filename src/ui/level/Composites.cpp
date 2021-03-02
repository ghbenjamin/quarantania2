#include <ui/level/Composites.h>
#include <ui/lib/Manager.h>

#include <ui/level/ActionPopupMenu.h>
#include <ui/level/EndTurnButton.h>


UI::BottomLeftBar::BottomLeftBar(UI::Manager *manager, UI::Element *parent)
        : Element(manager, parent)
{
    setLayout<HorizontalLayout>( 30, VAlignment::Centre );

    auto actionMenu = manager->createElement<UI::ActionMenu>(this);
    auto endTurn = manager->createElement<EndTurnButton>(this);
}
