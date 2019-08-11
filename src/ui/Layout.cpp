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

    auto preferred = ptr->preferredSize();

    for ( auto& c: ptr->children() )
    {
        c->setLocalPosition( Vector2i{0, h } );
        c->doLayout();

        auto s = c->outerSize();
        h += s.y();
        h += m_spacing;
        w = std::max( w, s.x() );
    }

    for ( auto& c: ptr->children() )
    {
        auto pos = c->localPosition();
        auto size = c->outerSize();

        switch (m_halign)
        {
            case HAlignment::Left:
                break;
            case HAlignment::Right:
                c->setLocalPosition({ w - size.x(), pos.y() });
                break;
            case HAlignment::Centre:
                c->setLocalPosition({ (w - size.x()) / 2, pos.y() });
                break;
            case HAlignment::Fill:
                c->setPreferredOuterSize({w, size.y()});
                break;
        }
    }

    if ( preferred != Vector2i::Null )
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

    return { w, h };
}

UI::VerticalLayout::VerticalLayout(int spacing, UI::HAlignment halign)
: m_spacing(spacing), m_halign(halign)
{

}

Vector2i UI::HorizontalLayout::doLayout(UI::Element *ptr)
{
    return Vector2i();
}

UI::HorizontalLayout::HorizontalLayout(int spacing, UI::VAlignment valign)
    : m_spacing(spacing), m_valign(valign)
{

}
