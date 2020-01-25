#include <ui/Layout.h>

#include <algorithm>

#include <ui/Element.h>
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

Vector2i UI::VerticalLayout::doLayout(UI::Element *ptr)
{
    int w = 0;
    int h = 0;

    auto preferred = ptr->preferredContentSize();

    // First pass - move our children to the right vertical positions
    for ( auto& c: ptr->children() )
    {
        c->setLocalPosition( Vector2i{0, h } );
        c->doLayout();

        auto s = c->outerSize();
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
    for ( auto& c: ptr->children() )
    {
        auto pos = c->localPosition();
        auto size = c->outerSize();

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
: m_spacing(spacing), m_halign(halign)
{

}

Vector2i UI::HorizontalLayout::doLayout(UI::Element *ptr)
{
    int w = 0;
    int h = 0;

    auto preferred = ptr->preferredContentSize();

    for ( auto& c: ptr->children() )
    {
        c->setLocalPosition( Vector2i{ w, 0 } );
        c->doLayout();

        auto s = c->outerSize();
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

    for ( auto& c: ptr->children() )
    {
        auto pos = c->localPosition();
        auto size = c->outerSize();

        switch (m_valign)
        {
            case VAlignment::Top:
                break;
            case VAlignment::Bottom:
                break;
            case VAlignment::Centre:
                break;
            case VAlignment::Fill:
                break;
        }
    }

    return { w, h };
}

UI::HorizontalLayout::HorizontalLayout(int spacing, UI::VAlignment valign)
    : m_spacing(spacing), m_valign(valign)
{

}
