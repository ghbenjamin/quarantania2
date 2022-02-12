#include <ui/level/OverworldMapDialog.h>
#include <ui/lib/Manager.h>
#include <ui/lib/ScrollHolder.h>
#include <ui/shared/OverworldView.h>

using namespace UI;

OverworldMapDialog::OverworldMapDialog( Manager *manager, Element *parent, Level *level, Overworld* overworld )
    : Element(manager, parent), m_level(level), m_overworld(overworld)
{
    manager->generateFromXML( "OverworldMapDialog", this );

    auto scrollHolder = withId<ScrollHolder>( "overworld-scroll" );
    scrollHolder->scrollToBottom();
    
    auto locationView = manager->createElement<UI::OverworldView>( scrollHolder.get(), m_overworld, true );
    
    doLayout();
    
    
    Logger::get().error( scrollHolder->outerBounds().right().to_string() );
    Logger::get().error( outerBounds().right().to_string() );
    
    addHotkey( SDLK_ESCAPE, [this](){
        deleteSelf();
    });
}
