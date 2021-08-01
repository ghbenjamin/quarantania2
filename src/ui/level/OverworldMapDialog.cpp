#include <ui/level/OverworldMapDialog.h>
#include <ui/lib/Manager.h>
#include <ui/lib/ScrollHolder.h>
#include <ui/shared/OverworldView.h>

using namespace UI;

OverworldMapDialog::OverworldMapDialog( Manager *manager, Element *parent, Level *level, Overworld* overworld )
    : Element(manager, parent), m_level(level), m_overworld(overworld)
{
    setLayout<CenterLayout>();
    setPreferredContentSize({1000, 800}); // TODO FIX ME
    setBackground(Colour::Teal);
    

    auto locationViewHolder = manager->createElement<UI::ScrollHolder>( this );
    locationViewHolder->setPreferredContentSize({1, 700});
    
    auto locationView = manager->createElement<UI::OverworldView>( locationViewHolder.get(), m_overworld, true );
    
    doLayout();
}
