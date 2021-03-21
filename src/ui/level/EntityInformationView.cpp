#include <ui/level/EntityInformationView.h>
#include <resource/ResourceManager.h>

UI::EntityInformationView::EntityInformationView( UI::Manager *manager, UI::Element *parent )
    : Element(manager, parent)
{
    setId("entity-information-view");
    setPreferredContentSize({300, 120});
    
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border" );
    NinePatch np = { patch.texture(), patch.offsets() };
    setBackground( np );
    setBorderWidth( patch.borderWidth() );
}

void UI::EntityInformationView::refresh( EntityRef entity )
{

}
