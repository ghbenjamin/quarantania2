#include <ui/level/BetterTextLog.h>
#include <ui/lib/Manager.h>
#include <resource/ResourceManager.h>

using namespace UI;


// Line data
// ------------------------------------


BTLLineData::BTLLineData( std::string const &data, Colour colour )
    : data(data), count(1), colour(colour) {}
    

// Scroll area
// ------------------------------------

BTLScrollArea::BTLScrollArea( Manager *manager, Element *parent )
     : Element(manager, parent), m_scrollPosition(1.0f)
{
    setLayout<TriggeredLayout>([this](){ layoutLines(); });
    setBoundsScissoring(true);
}

void BTLScrollArea::addLine( std::string const &data, Colour colour )
{
    auto newLine = manager()->createElement<Label>( this );
    m_lines.push_back(newLine);
    newLine->setMaximumOuterSize({innerBounds().w(), 0});
    newLine->setText( data );
    newLine->setTextColour( colour );
}

void BTLScrollArea::layoutLines()
{
    int maxY = 0;
    
    for ( auto const& l : m_lines )
    {
        maxY += l->outerBounds().h();
    }
    
    maxY += (m_lines.size() - 1) * LineSpacing;
    

    int currY = innerBounds().h();
    
    for (auto it = m_lines.rbegin(); it != m_lines.rend(); it++)
    {
        currY -= (*it)->outerBounds().h();
        currY -= LineSpacing;
        (*it)->setLocalPosition({0, currY});
    }
}

void BTLScrollArea::setScrollPosition(float position)
{
    m_scrollPosition = position;
    layoutLines();
}


// Scroll bar line
// ------------------------------------


void BTLScrollBarLine::onSizeSelf()
{
    if (m_sprite.has_value())
    {
        m_sprite->setTargetSize({ m_sprite->size().x(), innerBounds().h() });
    }
}



// Scroll bar
// ------------------------------------


BTLScrollBar::BTLScrollBar( Manager *manager, Element *parent )
    : Element(manager, parent), m_scrollPosition(1.0f)
{
    m_scrollbarLine = manager->createElement<BTLScrollBarLine>(this, "game_ui/scrollbar-line");
    m_scrollbarTop = manager->createElement<Icon>(this, "game_ui/scrollbar-top");
    m_scrollbarBottom = manager->createElement<Icon>(this, "game_ui/scrollbar-bottom");
    m_scrollbarHandle = manager->createElement<Icon>(this, "game_ui/scrollbar-handle");
    
    for (auto& item : { m_scrollbarTop, m_scrollbarBottom, m_scrollbarHandle } )
    {
        item->addEventCallback( UEventType::MouseIn, [item](UEvent& evt) {
            item->getSprite().setColourMod( Colour::Grey );
        });
    
        item->addEventCallback( UEventType::MouseOut, [item](UEvent& evt) {
            item->getSprite().setColourMod( Colour::White );
        });
    }
    
    addEventCallback( UEventType::Click, [this](UEvent& evt){
        onClick( evt.mouseButtonEvent.pos );
    });
    
    setPreferredContentWidth(m_scrollbarTop->outerBounds().w());
    setLayout<FreeLayout>();

    layoutElements();
}

void BTLScrollBar::layoutElements()
{
    auto size = preferredContentSize();
    auto scrollbarSize = m_scrollbarTop->outerBounds().right();
    
    // Place the scrollbar ends at the top and bottom of the element, centered horizontally
    m_scrollbarTop->setLocalPosition({( size.x() - scrollbarSize.x() ) / 2, 0});
    m_scrollbarBottom->setLocalPosition({( size.x() - scrollbarSize.x() ) / 2, size.y() - scrollbarSize.y()});
    
    // Stretch the scrollbar and centre it horizontally
    m_scrollbarLine->setLocalPosition({ (size.x() - m_scrollbarLine->outerBounds().w()) / 2, scrollbarSize.y()});
    m_scrollbarLine->setPreferredContentSize({m_scrollbarLine->outerBounds().w(), size.y() - 2 * scrollbarSize.y()});
    
    placeScrollHandle();
}

void BTLScrollBar::onSizeSelf()
{
    layoutElements();
}

void BTLScrollBar::onMoveSelf()
{
    layoutElements();
}

void BTLScrollBar::placeScrollHandle()
{
    auto size = preferredContentSize();
    auto scrollbarSize = m_scrollbarTop->outerBounds().right();

    // Layout the scrollhandle
    int scrollableRegion = size.y() - (2 * scrollbarSize.y()) - m_scrollbarHandle->outerBounds().h();
    int handleOffset = (int)( m_scrollPosition * (float)scrollableRegion );
    handleOffset += scrollbarSize.y();
    
    m_scrollbarHandle->setLocalPosition({( size.x() - m_scrollbarHandle->outerBounds().w() ) / 2, handleOffset });
}

void BTLScrollBar::setScrollPosition( float scrollPos )
{
    m_scrollPosition = scrollPos;
    placeScrollHandle();
}

void BTLScrollBar::onClick( Vector2i pos )
{
    auto localPos = pos - globalPosition();
    
    int topHandleHeight = m_scrollbarTop->outerBounds().h();
    int scrollLineHeight = m_scrollbarLine->outerBounds().h();
    
    auto scrollParent = closestAncestorWithTag( "scroll-parent" );
    Assert(scrollParent != nullptr);
    auto textLog = scrollParent->asType<BetterTextLog>();
    
    if ( localPos.y() >= topHandleHeight + scrollLineHeight )
    {
        // Bottom scroll button
        textLog->moveScrollPosition( ScrollPerTopBottomClick );
    }
    else if ( localPos.y() >= topHandleHeight )
    {
        // On the scroll line
        float frac = (float)(localPos.y() - topHandleHeight) / (float) scrollLineHeight;
        textLog->setScrollPosition(frac);
    }
    else
    {
        // Top scroll button
        textLog->moveScrollPosition( -ScrollPerTopBottomClick );
    }
    
}



// Main text log
// ------------------------------------

BetterTextLog::BetterTextLog( Manager *manager, Element *parent )
    : Element(manager, parent), m_scrollPosition(1.0f)
{
    
    setPreferredContentSize({350, 150});
    setLayout<HorizontalLayout>( 0, VAlignment::Fill );
    
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border" ).getPatch();
    setBackground( patch );
    setBorderWidth( patch.getBorderWidth() );
    
    setId("main-text-log");
    addTag("scroll-parent");
    
    m_scrollArea = manager->createElement<BTLScrollArea>(this);
    m_scrollbar = manager->createElement<BTLScrollBar>(this);
    
    m_scrollArea->setPreferredContentWidth( preferredContentSize().x() - m_scrollbar->outerBounds().w() );
}

void BetterTextLog::addLine( std::string const &data, Colour colour )
{
    m_scrollArea->addLine(data, colour);
}

void BetterTextLog::setScrollPosition( float pos )
{
    if (pos > 1.0f)
    {
        pos = 1.0f;
    }
    else if ( pos < 0.0001f )
    {
        pos = 0.0f;
    }
    
    m_scrollPosition = pos;
    m_scrollbar->setScrollPosition(m_scrollPosition);
    m_scrollArea->setScrollPosition(m_scrollPosition);
}

void BetterTextLog::moveScrollPosition( float dy )
{
    setScrollPosition( m_scrollPosition + dy );
}
