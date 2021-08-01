#include <ui/level/OverworldMapDialog.h>
#include <ui/lib/Manager.h>
#include <ui/lib/ScrollHolder.h>
#include <ui/shared/OverworldView.h>
#include <resource/ResourceManager.h>

using namespace UI;

OverworldMapDialog::OverworldMapDialog( Manager *manager, Element *parent, Level *level, Overworld* overworld )
    : Element(manager, parent), m_level(level), m_overworld(overworld)
{
    setLayout<CenterLayout>();
    setPreferredContentSize({1000, 800}); // TODO FIX ME
    
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border" ).getPatch();
    setBackground( patch );
    setBorderWidth( patch.getBorderWidth() );

    auto locationViewHolder = manager->createElement<UI::ScrollHolder>( this );
    locationViewHolder->setPreferredContentSize({1, 700});
    
    auto locationView = manager->createElement<UI::OverworldView>( locationViewHolder.get(), m_overworld, true );
    
    doLayout();
    
    addHotkey( SDLK_ESCAPE, [this](){
        deleteSelf();
    });
}
