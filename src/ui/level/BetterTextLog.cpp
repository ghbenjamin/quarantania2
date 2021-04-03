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
    
    for (auto& item : { m_scrollbarTop, m_scrollbarBottom } )
    {
        item->addEventCallback( UEventType::MouseIn, [item](UEvent& evt) {
            item->getSprite().setColourMod( Colour::Grey );
        });
    
        item->addEventCallback( UEventType::MouseOut, [item](UEvent& evt) {
            item->getSprite().setColourMod( Colour::White );
        });
    }
    
    addEventCallback( UEventType::Click, [this](UEvent& evt){
        // TBD
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
    m_scrollbarLine->setLocalPosition({ (size.x() - m_scrollbarLine->outerBounds().w()) / 2, scrollbarSize.y() / 2 });
    m_scrollbarLine->setPreferredContentSize({m_scrollbarLine->outerBounds().w(), size.y() - scrollbarSize.y()});
    
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



// Main text log
// ------------------------------------

BetterTextLog::BetterTextLog( Manager *manager, Element *parent )
    : Element(manager, parent)
{
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border" );
    NinePatch np = { patch.texture(), patch.offsets() };

    setPreferredContentSize({350, 150});
    setLayout<HorizontalLayout>( 0, VAlignment::Fill );
    
    setBackground( np );
    setBorderWidth( patch.borderWidth() );
    
    setId("main-text-log");
    
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
    m_scrollPosition = pos;
}