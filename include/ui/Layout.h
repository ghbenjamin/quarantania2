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

Vector2i alignRectWithinRect( Vector2i const& outer, Vector2i const& inner, Alignment alignment );

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
    Vector2i doLayout(Element *ptr) override;
};


class HorizontalLayout : public ElementLayout
{
public:
    Vector2i doLayout(Element *ptr) override;
};



}