#include <ui/overworld/LocationView.h>
#include <ui/lib/Manager.h>
#include <game/Overworld.h>
#include <resource/ResourceManager.h>

using namespace UI;


LocationViewItem::LocationViewItem( Manager *manager, Element *parent, OverworldLocation const *loc )
        : Element(manager, parent), m_loc(loc)
{
    SpritesheetKey locSprite;
    switch ( loc->type )
    {
        case OverworldLocationType::Fight:
            locSprite = "game_ui/w-position-marker";
            break;
        case OverworldLocationType::Shop:
            locSprite = "game_ui/w-position-marker";
            break;
        case OverworldLocationType::Mystery:
            locSprite = "game_ui/w-position-marker";
            break;
        case OverworldLocationType::Boss:
            locSprite = "game_ui/w-position-marker";
            break;
    }
    
    m_icon = manager->createElement<Icon>( this, locSprite );
    m_icon->getSprite().setPermanentColour(Colour::Black);
    
    
    addEventCallback( UEventType::MouseIn, [this](UEvent const& evt) {
        m_icon->getSprite().setColourMod( Colour::Red );
        ResourceManager::get().getWindow()->cursor().setCursorType( CursorType::Interact );
    });
    
    addEventCallback( UEventType::MouseOut, [this](UEvent const& evt) {
        m_icon->getSprite().resetColourMod();
        ResourceManager::get().getWindow()->cursor().resetCursor();
    });
}

void LocationViewItem::refresh()
{

}

LocationView::LocationView( Manager *manager, Element *parent, Overworld *overworld )
: Element( manager, parent ), m_overworld(overworld)
{
    Vector2i scaling = {80, 100};

    setPreferredContentSize({ scaling.x() * overworld->gridSize().x(), scaling.y()  * overworld->gridSize().y() });
    setLayout<FreeLayout>();
    setBackground(Colour::White.withAlphaF(0.5));

    
    for ( auto const& loc : overworld->locations() )
    {
        Vector2i pos = loc.gridPos.hadamard( scaling );
        auto locItem = manager->createElement<LocationViewItem>( this, &loc );
        locItem->setLocalPosition( pos );

        m_locations.push_back(locItem);
    }
}

void LocationView::refresh()
{
    for ( auto const loc : m_locations )
    {
        loc->refresh();
    }
}
