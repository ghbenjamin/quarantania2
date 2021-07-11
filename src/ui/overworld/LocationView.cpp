#include <ui/overworld/LocationView.h>
#include <ui/lib/Manager.h>
#include <game/Overworld.h>
#include <resource/ResourceManager.h>
#include <graphics/Primatives.h>
#include <utils/Math.h>
#include <graphics/RenderInterface.h>

using namespace UI;


LocationViewItem::LocationViewItem( Manager *manager, Element *parent, Overworld* overworld, OverworldLocation const *loc )
        : Element(manager, parent), m_loc(loc), m_overworld(overworld)
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
    
    addEventCallback( UEventType::Click, [this](UEvent const& evt) {
        this->onClick();
    });
}

void LocationViewItem::refresh()
{

}

void LocationViewItem::onClick()
{
    m_overworld->events().broadcast<GameEvents::OverworldLocationSelect>( m_loc->index );
}

LocationView::LocationView( Manager *manager, Element *parent, Overworld *overworld )
: Element( manager, parent ), m_overworld(overworld)
{
    Vector2i scaling = {80, 100};

    setPreferredContentSize({ scaling.x() * overworld->gridSize().x(), scaling.y()  * overworld->gridSize().y() });
    setLayout<FreeLayout>();
    setBackground(Colour::White.withAlphaF(0.5));

    // Create overworld location elements
    for ( auto const& loc : overworld->locations() )
    {
        Vector2i pos = loc.gridPos.hadamard( scaling );
        auto locItem = manager->createElement<LocationViewItem>( this, m_overworld, &loc );
        locItem->setLocalPosition( pos );

        m_locations.push_back(locItem);
    }
    
    // Create overworld connection elements
    for ( auto const [startIdx, endIdx] : overworld->connections() )
    {
        auto& startLoc = m_locations[startIdx];
        auto& endLoc = m_locations[endIdx];
    
        Vector2i startLocPos = startLoc->localPosition();
        Vector2i endLocPos = endLoc->localPosition();
        Vector2i startLocHalfSize = startLoc->outerBounds().right() / 2;
        Vector2i endLocHalfSize = endLoc->outerBounds().right() / 2;
        Vector2i startCenter( startLocPos + startLocHalfSize );
        Vector2i endCenter( endLocPos + endLocHalfSize );
    
        float dist = Math::pointDistance(startCenter, endCenter);
        int rectWidth = 3;
        int rectHeight = (int) dist - 40;
        Vector2i rectSize = { rectWidth, rectHeight };
    
        auto sprite = createRectangle( rectSize, Colour::Red );
        auto centrePoint = Math::centrePoint( startCenter, endCenter );
        
        centrePoint = centrePoint - rectSize / 2;
        
        
        auto delta = endLocPos - startLocPos;
        
        float angle = -std::atan2( delta.x(), delta.y() );
        sprite.setRotation(angle);
        
        m_connections.emplace( centrePoint, sprite );
    }
}

void LocationView::refresh()
{
    for ( auto const loc : m_locations )
    {
        loc->refresh();
    }
}

void LocationView::updateSelf( uint32_t ticks, InputInterface &iinter, RenderInterface &rInter )
{
    auto globalPos = globalPosition() + contentOffset();
    for ( auto& [k, v] : m_connections )
    {
        Vector2i localPos = globalPos + k;
        rInter.addItem( v.renderObject(localPos), RenderLayer::UI );
    }
}
