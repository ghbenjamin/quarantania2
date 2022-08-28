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
        manager->createElement<Label>( tableContainer.get(), std::to_string((int)mod.value) );
    }

    doLayout();
}