#pragma once

#include <ui/lib/Element.h>
#include <ui/lib/Controls.h>


class Overworld;
struct OverworldLocation;

namespace UI
{


class LocationViewItem : public Element
{
public:
    LocationViewItem( Manager *manager, Element *parent, OverworldLocation const* loc );

private:
    OverworldLocation const* m_loc;
    std::shared_ptr<Icon> m_icon;
};


class LocationPathItem : public Element
{
public:
    LocationPathItem( Manager *manager, Element *parent, Vector2i start, Vector2i end );

private:
    std::shared_ptr<Icon> m_icon;
};


class LocationView : public Element
{
public:
    LocationView( Manager *manager, Element *parent, Overworld* overworld );
    ~LocationView() override = default;

private:
    Overworld* m_overworld;
};


}