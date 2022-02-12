#include <ui/lib/Layout.h>

#include <algorithm>

#include <ui/lib/Element.h>
#include <ui/lib/ScrollHolder.h>
#include <utils/Assert.h>

using namespace UI;


// Utils
// ----------------------------------

Vector2i UI::alignRectWithinRect(Vector2i const &outer, Vector2i const &inner, Alignment alignment)
{
    Assert( outer.x() >= inner.x() );
    Assert( outer.y() >= inner.y() );

    int centreX = ( outer.x() - inner.x() ) / 2;
    int centreY = ( outer.y() - inner.y() ) / 2;

    int rightX = outer.x() - inner.x();
    int bottomY = outer.y() - inner.y();

    Vector2i out;

    switch (alignment)
    {
        case Alignment::TopLeft:
            out = {0, 0};
            break;
        case Alignment::TopCentre:
            out = { centreX, 0 };
            break;
        case Alignment::TopRight:
            out = { rightX, 0 };
            break;
        case Alignment::CentreRight:
            out = { rightX, centreY };
            break;
        case Alignment::BottomRight:
            out = { rightX, bottomY };
            break;
        case Alignment::BottomCentre:
            out = { centreX, bottomY };
            break;
        case Alignment::BottomLeft:
            out = { 0, bottomY };
            break;
        case Alignment::CentreLeft:
            out = { 0, centreY };
            break;
        case Alignment::Centre:
            out = { centreX, centreY };
            break;
    }

    return out;
}

Vector2i UI::alignRectToRect( Vector2i const& dep, Vector2i const& fixed, Alignment alignment )
{
    std::optional<Vector2i> out;

    switch ( alignment )
    {
        case Alignment::TopCentre:
            out = { (fixed.x() - dep.x()) / 2, -dep.y() };
            break;

        case Alignment::TopLeft:
            out = { 0, -dep.y() };
            break;

        case Alignment::TopRight:
            out = { fixed.x() - dep.x(), -dep.y() };
            break;

        case Alignment::BottomRight:
            out = { fixed.x() - dep.x(), fixed.y() };
            break;

        case Alignment::BottomCentre:
            break;

        case Alignment::BottomLeft:
            break;

        case Alignment::CentreRight:
            break;

        case Alignment::CentreLeft:
            break;

        case Alignment::Centre:
            break;
    }

    if ( !out.has_value() )
    {
        AssertNotImplemented();
    }

    return *out;
}


// Base class
// ---------------------------------

ElementLayout::ElementLayout( Element *elem )
        : m_element(elem) {}


// Vertical layout
// ---------------------------------

Vector2i VerticalLayout::doLayout()
{
    int w = 0;
    int h = 0;

    auto preferred = m_element->preferredContentSize();

    // First pass - move our children to the right vertical positions
    for ( auto const& c: m_element->children() )
    {
        c->setLocalPosition( Vector2i{0, h } );
        c->doLayout();

        auto s = c->outerBounds().right();
        h += s.y();
        h += m_spacing;
        w = std::max( w, s.x() );
    }
    
    // Don't add extra spacing to the end of the final element
    if ( m_element->children().size() > 0 )
    {
        h -= m_spacing;
    }

    // If the max width / total height of the children is smaller than our preferred
    // content size, set our actual size to our content size (avoid being too small)
    if ( preferred != Vector2i{0, 0} )
    {
        if ( w < preferred.x() )
        {
            w = preferred.x();
        }

        if ( h <  preferred.y() )
        {
            h = preferred.y();
        }
    }

    // Second pass over children - set the correct horizontal size & position
    for ( auto const& c: m_element->children() )
    {
        auto pos = c->localPosition();
        auto size = c->outerBounds().right();

        switch (m_halign)
        {
            // For left/right/centre we use the child's preferred size - no changes in size
            // based on the size of the parent

            case HAlignment::Left:
                break;
            case HAlignment::Right:
                c->setLocalPosition({ w - size.x(), pos.y() });
                break;
            case HAlignment::Centre:
                c->setLocalPosition({ (w - size.x()) / 2, pos.y() });
                break;

            // For fill we change the (horizontal) size of the child based on the current size of the parent

            case HAlignment::Fill:
                c->setPreferredOuterSize({w, size.y()});
                break;
        }
    }

    return { w, h };
}

VerticalLayout::VerticalLayout(Element* elem, int spacing, HAlignment halign)
: ElementLayout(elem), m_spacing(spacing), m_halign(halign) {}


// Horizontal layout
// ---------------------------------


Vector2i HorizontalLayout::doLayout()
{
    int w = 0;
    int h = 0;

    auto preferred = m_element->preferredContentSize();

    for ( auto const& c: m_element->children() )
    {
        c->setLocalPosition( Vector2i{ w, 0 } );
        c->doLayout();

        auto s = c->outerBounds().right();
        w += s.x();
        w += m_spacing;
        h = std::max( h, s.y() );
    }
    
    // Don't add extra spacing to the end of the final element
    if ( m_element->children().size() > 0 )
    {
        w -= m_spacing;
    }

    if ( preferred != Vector2i{0, 0} )
    {
        if ( w < preferred.x() )
        {
            w = preferred.x();
        }

        if ( h <  preferred.y() )
        {
            h = preferred.y();
        }
    }

    for ( auto const& c: m_element->children() )
    {
        auto pos = c->localPosition();
        auto size = c->outerBounds().right();

        switch (m_valign)
        {
            case VAlignment::Top:
                break;
            case VAlignment::Bottom:
                c->setLocalPosition({ pos.x(), pos.y() + h - size.y() });
                break;
            case VAlignment::Centre:
                c->setLocalPosition({ pos.x(), pos.y() + (h / 2) - (size.y() / 2) });
                break;

            // For fill we change the (vertical) size of the child based on the current size of the parent

            case VAlignment::Fill:
                c->setPreferredOuterSize({size.x(), h});
                break;
        }
    }

    return { w, h };
}

HorizontalLayout::HorizontalLayout(Element* elem, int spacing, VAlignment valign)
    : ElementLayout(elem), m_spacing(spacing), m_valign(valign) {}


// Free layout
// ---------------------------------


Vector2i FreeLayout::doLayout()
{
    for (auto const& c : m_element->children() )
    {
        c->doLayout();
    }

    return m_element->preferredContentSize();
}

FreeLayout::FreeLayout( Element *elem )
    : ElementLayout(elem) {}


// Center layout
// ---------------------------------

CenterLayout::CenterLayout( Element *elem )
    : ElementLayout(elem) {}
    
Vector2i CenterLayout::doLayout()
{
    Assert( m_element->children().size() == 1 );

    auto contentSize = m_element->preferredContentSize();
    auto& child = m_element->children().front();

    child->doLayout();

    auto childBounds = child->outerBounds();


    // If we have no preferred size, stretch to the size of our child 
    if ( contentSize == Vector2i::Null() )
    {
        contentSize = childBounds.right();
    }

    child->setLocalPosition({
        contentSize.x() / 2 - childBounds.w() / 2,
        contentSize.y() / 2 - childBounds.h() / 2,
    });

    return contentSize;
}


// HorizontalSpaceBetweenLayout layout
// ----------------------------------

HorizontalSpaceBetweenLayout::HorizontalSpaceBetweenLayout(Element* elem, VAlignment valign )
 : ElementLayout(elem), m_valign(valign) {}
 
Vector2i HorizontalSpaceBetweenLayout::doLayout()
{
    int w = 0;
    int h = 0;
    
    if ( m_element->children().size() <= 1 )
    {
        return {w, h};
    }
    
    auto preferred = m_element->preferredContentSize();
    
    for ( auto const& c: m_element->children() )
    {
        c->setLocalPosition( Vector2i{ 0, 0 } );
        c->doLayout();
        
        auto s = c->outerBounds().right();
        w += s.x();
        h = std::max( h, s.y() );
    }
    
    int totalChildW = w;
    
    if ( preferred != Vector2i{0, 0} )
    {
        if ( w < preferred.x() )
        {
            w = preferred.x();
        }
        
        if ( h <  preferred.y() )
        {
            h = preferred.y();
        }
    }
    
    int childSpacing = ( w - totalChildW ) / (m_element->children().size() - 1);
    
    int x = 0;
    
    for ( auto const& c: m_element->children() )
    {
        auto pos = c->localPosition();
        auto size = c->outerBounds().right();
        
        switch (m_valign)
        {
            case VAlignment::Top:
                break;
            case VAlignment::Bottom:
                c->setLocalPosition({ x, pos.y() + h - size.y() });
                break;
            case VAlignment::Centre:
                c->setLocalPosition({ x, pos.y() + (h / 2) - (size.y() / 2) });
                break;
            case VAlignment::Fill:
                c->setLocalPosition({ x, 0 });
                c->setPreferredOuterSize({size.x(), h});
                break;
        }
        
        x += childSpacing;
        x += size.x();
    }
    
    return { w, h };
}


// Grid layout
// ---------------------------------


GridLayout::GridLayout(Element* elem, Vector2i gridDimensions, Vector2i itemSize, int itemSpacing )
    : ElementLayout(elem), m_gridDimensions(gridDimensions), m_itemSize(itemSize), m_itemSpacing(itemSpacing) {}

Vector2i GridLayout::doLayout()
{
    Assert( m_gridDimensions.x() > 0 && m_gridDimensions.y() > 0 );

    auto currChild = m_element->children().begin();
    bool shouldBreak = false;
    
    for (int j = 0; j < m_gridDimensions.y() && !shouldBreak; j++)
    {
        for (int i = 0; i < m_gridDimensions.x() && !shouldBreak; i++)
        {
            Vector2i pos = {
                (m_itemSize.x() + m_itemSpacing) * i,
                (m_itemSize.y() + m_itemSpacing) * j
            };
            
            (*currChild)->setLocalPosition( pos );
            
            currChild++;
    
            if (currChild == m_element->children().end() )
            {
                shouldBreak = true;
            }
        }
    }
    
    
    int sizeX = m_gridDimensions.x() * m_itemSize.x() + (m_gridDimensions.x() - 1) * m_itemSpacing;
    int sizeY = m_gridDimensions.y() * m_itemSize.y() + (m_gridDimensions.y() - 1) * m_itemSpacing;
    
    return { sizeX, sizeY };
}



// Triggered layout
// ----------------------------------

TriggeredLayout::TriggeredLayout( Element *elem, std::function<void()> func )
    : ElementLayout(elem), m_callback(func) {}

Vector2i TriggeredLayout::doLayout()
{
    for (auto const& c : m_element->children() )
    {
        c->doLayout();
    }
    
    m_callback();
    
    return m_element->preferredContentSize();
}

VScrollLayout::VScrollLayout(ScrollHolder *elem)
        : ElementLayout(elem), m_scrollHolder(elem) {}

Vector2i VScrollLayout::doLayout()
{
    Assert( m_element->children().size() == 1 );

    // Get our size
    auto contentSize = m_element->preferredContentSize();

    // Get our child size
    auto& child = m_element->children().front();
    child->doLayout();
    auto childBounds = child->outerBounds();

    // We need to be at least as large as our child
    if ( contentSize.x() < childBounds.w() )
    {
        contentSize.x( childBounds.w() );
    }


    int scrollHeightDiff = childBounds.h() - contentSize.y();
    float scrollPercent = m_scrollHolder->getScrollPercentage();
    int scrollOffset = (int)( scrollPercent * (float) scrollHeightDiff );

    child->setLocalPosition({
            contentSize.x() / 2 - childBounds.w() / 2,  // Center horizontally
            -scrollOffset
    });

    return contentSize;
}
