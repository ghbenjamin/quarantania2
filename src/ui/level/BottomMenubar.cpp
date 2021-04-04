#include <ui/level/BottomMenubar.h>
#include <ui/lib/Manager.h>

using namespace UI;

BottomMenubar::BottomMenubar( Manager *manager, Element *parent ) : Element(manager, parent)
{
    setLayout<HorizontalLayout>( 2, VAlignment::Centre );
    setBackground(Colour::Olive);
    manager->createElement<IconButton>( this, "game_ui/axe-sword", [](){} );
    manager->createElement<IconButton>( this, "game_ui/axe-sword", [](){} );
}
