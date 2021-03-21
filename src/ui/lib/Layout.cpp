#include <ui/lib/Layout.h>

#include <algorithm>

#include <ui/lib/Element.h>
#include <utils/Assert.h>

Vector2i UI::alignRectWithinRect(Vector2i const &outer, Vector2i const &inner, UI::Alignment alignment)
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



Vector2i UI::VerticalLayout::doLayout(UI::Element *ptr)
{
    int w = 0;
    int h = 0;

    auto preferred = ptr->preferredContentSize();

    // First pass - move our children to the right vertical positions
    for ( auto const& c: ptr->children() )
    {
        c->setLocalPosition( Vector2i{0, h } );
        c->doLayout();

        auto s = c->outerBounds().right();
        h += s.y();
        h += m_spacing;
        w = std::max( w, s.x() );
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
    for ( auto const& c: ptr->children() )
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

UI::VerticalLayout::VerticalLayout(int spacing, UI::HAlignment halign)
: m_spacing(spacing), m_halign(halign) {}

Vector2i UI::HorizontalLayout::doLayout(UI::Element *ptr)
{
    int w = 0;
    int h = 0;

    auto preferred = ptr->preferredContentSize();

    for ( auto const& c: ptr->children() )
    {
        c->setLocalPosition( Vector2i{ w, 0 } );
        c->doLayout();

        auto s = c->outerBounds().right();
        w += s.x();
        w += m_spacing;
        h = std::max( h, s.y() );
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

    for ( auto const& c: ptr->children() )
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

UI::HorizontalLayout::HorizontalLayout(int spacing, UI::VAlignment valign)
    : m_spacing(spacing), m_valign(valign) {}

Vector2i UI::FreeLayout::doLayout(UI::Element *ptr)
{
    for (auto const& c : ptr->children() )
    {
        c->doLayout();
    }

    return ptr->preferredContentSize();
}

Vector2i UI::CenterLayout::doLayout(UI::Element *ptr)
{
    Assert( ptr->children().size() == 1 );

    auto contentSize = ptr->preferredContentSize();
    auto& child = ptr->children().front();

    child->doLayout();

    auto childBounds = child->outerBounds();

    child->setLocalPosition({
        contentSize.x() / 2 - childBounds.w() / 2,
        contentSize.y() / 2 - childBounds.h() / 2,
    });

    return contentSize;
}

UI::HorizontalSpaceBetweenLayout::HorizontalSpaceBetweenLayout( UI::VAlignment valign )
 : m_valign(valign) {}

Vector2i UI::HorizontalSpaceBetweenLayout::doLayout( UI::Element *ptr )
{
    int w = 0;
    int h = 0;
    
    if ( ptr->children().size() <= 1 )
    {
        return {w, h};
    }
    
    auto preferred = ptr->preferredContentSize();
    
    for ( auto const& c: ptr->children() )
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
    
    int childSpacing = ( w - totalChildW ) / (ptr->children().size() - 1);
    
    int x = 0;
    
    for ( auto const& c: ptr->children() )
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


UI::GridLayout::GridLayout( Vector2i gridDimensions, Vector2i itemSize, int itemSpacing )
    : m_gridDimensions(gridDimensions), m_itemSize(itemSize), m_itemSpacing(itemSpacing) {}

Vector2i UI::GridLayout::doLayout( UI::Element *ptr )
{
    Assert( m_gridDimensions.x() > 0 && m_gridDimensions.y() > 0 );

    auto currChild = ptr->children().begin();
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
    
            if (currChild == ptr->children().end() )
            {
                shouldBreak = true;
            }
        }
    }
    
    
    int sizeX = m_gridDimensions.x() * m_itemSize.x() + (m_gridDimensions.x() - 1) * m_itemSpacing;
    int sizeY = m_gridDimensions.y() * m_itemSize.y() + (m_gridDimensions.y() - 1) * m_itemSpacing;
    
    return { sizeX, sizeY };
}