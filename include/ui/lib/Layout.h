#pragma once

#include <utils/Containers.h>
#include <functional>

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
class ScrollHolder;

class ElementLayout
{
public:

    ElementLayout(Element* elem);
    virtual ~ElementLayout() = default;

    virtual Vector2i doLayout() = 0;

protected:
    Element* m_element;
};


class VerticalLayout : public ElementLayout
{
public:
    VerticalLayout(Element* elem, int spacing = 0, HAlignment halign=HAlignment::Left);
    ~VerticalLayout() override = default;

    Vector2i doLayout() override;

private:
    int m_spacing;
    HAlignment m_halign;
};


class HorizontalLayout : public ElementLayout
{
public:
    HorizontalLayout(Element* elem, int spacing = 0, VAlignment valign=VAlignment::Centre);
    ~HorizontalLayout() override = default;

    Vector2i doLayout() override;

private:
    int m_spacing;
    VAlignment m_valign;
};


class HorizontalSpaceBetweenLayout : public ElementLayout
{
public:
    HorizontalSpaceBetweenLayout(Element* elem, VAlignment valign=VAlignment::Centre);
    ~HorizontalSpaceBetweenLayout() override = default;
    
    Vector2i doLayout() override;

private:
    VAlignment m_valign;
};


class CenterLayout : public ElementLayout
{
public:
    CenterLayout(Element* elem);
    ~CenterLayout() override = default;
    
    Vector2i doLayout() override;
};


class FreeLayout : public ElementLayout
{
public:
    FreeLayout(Element* elem);
    ~FreeLayout() override = default;

    Vector2i doLayout() override;
};


class GridLayout : public ElementLayout
{
public:
    GridLayout(Element* elem, Vector2i gridDimensions, Vector2i itemSize, int itemSpacing );
    ~GridLayout() = default;
    
    Vector2i doLayout() override;

private:
    Vector2i m_gridDimensions;
    Vector2i m_itemSize;
    int m_itemSpacing;
};



class TriggeredLayout : public ElementLayout
{
public:
    TriggeredLayout(Element* elem, std::function<void()> func);
    ~TriggeredLayout() override = default;
    
    Vector2i doLayout() override;

private:
    std::function<void()> m_callback;
};


class VScrollLayout : public ElementLayout
{
public:
    VScrollLayout(ScrollHolder* elem);
    ~VScrollLayout() = default;

    Vector2i doLayout() override;

private:
    ScrollHolder* m_scrollHolder;
};


}