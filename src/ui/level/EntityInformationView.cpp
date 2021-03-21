#include <ui/level/EntityInformationView.h>
#include <resource/ResourceManager.h>

UI::EntityInformationView::EntityInformationView( UI::Manager *manager, UI::Element *parent )
    : Element(manager, parent)
{
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border" );
    NinePatch np = { patch.texture(), patch.offsets() };
    setBackground( np );
    setBorderWidth( patch.borderWidth() );
}
