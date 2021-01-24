#pragma once

#include <utils/Containers.h>

namespace UI
{

enum class Alignment
{
    TopLeft,
    TopCentre,
    TopRight,
    CentreRight,
    BottomRight,
    BottomCentre,
    BottomLeft,
    CentreLeft,
    Centre
};

enum class VAlignment
{
    Top,
    Bottom,
    Centre,
    Fill
};

enum class HAlignment
{
    Left,
    Right,
    Centre,
    Fill
};

// Accept a pair of rectangle sizes. Return the coordinates of the inner rectangle relative to the outer rectangle
// if the inner rectangle is placed within the outer rectangle at the given alignment.
Vector2i alignRectWithinRect( Vector2i const& outer, Vector2i const& inner, Alignment alignment );

// Accept a pair of rectangles. Return the coordinates of the dependant rectangle relative to the fixed rectangle
// if the dependent rectangle is placed alongside the fixed rectangle at the given alignment.
Vector2i alignRectToRect( Vector2i const& dep, Vector2i const& fixed, Alignment alignment );

class Element;

class ElementLayout
{
public:

    ElementLayout() = default;
    virtual ~ElementLayout() = default;

    virtual Vector2i doLayout( Element* ptr ) = 0;
};


class VerticalLayout : public ElementLayout
{
public:
    VerticalLayout(int spacing = 0, HAlignment halign=HAlignment::Left);
    ~VerticalLayout() override = default;

    Vector2i doLayout(Element *ptr) override;

private:
    int m_spacing;
    HAlignment m_halign;
};


class HorizontalLayout : public ElementLayout
{
public:
    HorizontalLayout(int spacing = 0, VAlignment valign=VAlignment::Centre);
    ~HorizontalLayout() override = default;

    Vector2i doLayout(Element *ptr) override;

private:
    int m_spacing;
    VAlignment m_valign;
};

class CenterLayout : public ElementLayout
{
public:
    Vector2i doLayout(Element *ptr) override;
};


class FreeLayout : public ElementLayout
{
public:
    FreeLayout() = default;
    ~FreeLayout() = default;

    Vector2i doLayout(Element *ptr) override;
};


}