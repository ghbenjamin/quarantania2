#include <ui/level/ModifierOverlay.h>
#include <ui/lib/Manager.h>
#include <ui/lib/Controls.h>
#include <game/Level.h>
#include <game/ActorModifier.h>

using namespace UI;

ModifierOverlay::ModifierOverlay(Manager *manager, Element* parent, Level* level, ActorCalcData const& data)
    : Element(manager, parent), m_level(level)
{
    manager->generateFromLua( "ui/elements/ModifierOverlay", this );

    auto title = withId<Label>( "modifier-overlay-title" );
    title->setText("Modifiers");

    auto tableContainer = withId( "modifier-list-container" );

    for ( auto const& mod : data.mods.items() )
    {
        int value = (int) mod.value;
        std::string prefix;
        
        if ( value < 0 )
        {
            prefix = "-";
        }
        else if ( value > 0 )
        {
            prefix = "+";
        }
        else
        {
            prefix = "";
        }
        
        std::string modifierText = prefix + std::to_string(value);
    
        
        auto rowElement = manager->createElement( tableContainer.get() );
        rowElement->setLayout<HorizontalLayout>(4, VAlignment::Centre);
        
        manager->createElement<Label>( rowElement.get(), std::string(mod.source) );
        manager->createElement<Label>( rowElement.get(), modifierText );
    }

    doLayout();
}