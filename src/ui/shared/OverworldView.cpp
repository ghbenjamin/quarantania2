#include <ui/shared/OverworldView.h>
#include <ui/lib/Manager.h>
#include <game/Overworld.h>
#include <resource/ResourceManager.h>
#include <graphics/Primatives.h>
#include <utils/Math.h>
#include <graphics/RenderInterface.h>

using namespace UI;


OverworldViewLocationItem::OverworldViewLocationItem( Manager *manager, Element *parent, Overworld* overworld, OverworldLocation const *loc, bool readOnly )
        : Element(manager, parent), m_loc(loc), m_overworld(overworld), m_readOnly(readOnly)
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
            locSprite = "game_ui/w-crowned-skull";
            break;
    }
    
    m_icon = manager->createElement<Icon>( this, locSprite );
    
    
    if ( m_overworld->nextLocations().count(m_loc->idx) && !m_readOnly )
    {
        m_icon->getSprite().setPermanentColour(Colour::Green);
    }
    else if ( m_overworld->currentLocation() == m_loc->idx )
    {
        m_icon->getSprite().setPermanentColour(Colour::Blue);
    }
    else
    {
        m_icon->getSprite().setPermanentColour(Colour::Black);
    }
    
    
    addEventCallback( UEventType::MouseIn, [this](UEvent const& evt) { onMouseIn(); });
    addEventCallback( UEventType::MouseOut, [this](UEvent const& evt) { onMouseOut(); });
    addEventCallback( UEventType::Click, [this](UEvent const& evt) { onClick(); });
}

void OverworldViewLocationItem::refresh()
{

}

void OverworldViewLocationItem::onClick()
{
    if ( !m_readOnly )
    {
        if ( m_overworld->nextLocations().count(m_loc->idx) )
        {
            m_overworld->events().broadcast<GameEvents::OverworldLocationSelect>( m_loc->idx );
        }
    }
}

void OverworldViewLocationItem::onMouseIn()
{
    if ( !m_readOnly )
    {
        m_icon->getSprite().setColourMod(Colour::Red);
        ResourceManager::get().getWindow()->cursor().setCursorType(CursorType::Interact);
    }
}

void OverworldViewLocationItem::onMouseOut()
{
    if ( !m_readOnly )
    {
        m_icon->getSprite().resetColourMod();
        ResourceManager::get().getWindow()->cursor().resetCursor();
    }
}

OverworldView::OverworldView( Manager *manager, Element *parent, Overworld *overworld, bool readOnly )
: Element( manager, parent ), m_overworld(overworld), m_readOnly(readOnly)
{
    Vector2i scaling = {80, 100};
    Vector2i contentSize = scaling.elemProduct(overworld->gridSize() - Vector2i{0, 1}) + Vector2i(0, 32);
    
    setPreferredContentSize( contentSize );
    setLayout<FreeLayout>();
    setBackground(Colour::White.withAlphaF(0.5));
    setPadding(20);

    // Create overworld location elements
    for ( auto const& loc : overworld->locations() )
    {
        Vector2i pos = loc.gridPos.elemProduct(scaling);
        auto locItem = manager->createElement<OverworldViewLocationItem>(this, m_overworld, &loc, m_readOnly );
        
        locItem->setLocalPosition( { pos.x(), contentSize.y() - pos.y() - locItem->outerBounds().h() } );
        
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

        addConnectionLine( startCenter, endCenter, 3, 40 );
    }
}

void OverworldView::refresh()
{
    for ( auto const loc : m_locations )
    {
        loc->refresh();
    }
}

void OverworldView::updateSelf( uint32_t ticks, InputInterface &iinter, RenderInterface &rInter )
{
    auto globalPos = globalPosition() + contentOffset();
    for ( auto& [k, v] : m_connections )
    {
        Vector2i localPos = globalPos + k;
        rInter.addItem( v.renderObject(localPos), RenderLayer::UI );
    }
}

void OverworldView::addConnectionLine( Vector2i startLoc, Vector2i endLoc, int width, int margins)
{
    float dist = Math::pointDistance(startLoc, endLoc);
    int rectHeight = (int) dist - margins;
    Vector2i rectSize = { width, rectHeight };

    auto sprite = createRectangle( rectSize, Colour::Grey.withAlphaF( 0.5f ) );
    auto centrePoint = Math::centrePoint( startLoc, endLoc );

    centrePoint = centrePoint - rectSize / 2;

    auto delta = endLoc - startLoc;
    float angle = (float) -std::atan2( delta.x(), delta.y() );
    sprite.setRotation(angle);

    m_connections.emplace( centrePoint, sprite );
}
