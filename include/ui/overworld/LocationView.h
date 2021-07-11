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
    LocationViewItem( Manager *manager, Element *parent, Overworld* overworld, OverworldLocation const* loc );

    void refresh();
    void onClick();

private:
    OverworldLocation const* m_loc;
    Overworld* m_overworld;
    std::shared_ptr<Icon> m_icon;
};


class LocationView : public Element
{
public:
    LocationView( Manager *manager, Element *parent, Overworld* overworld );
    ~LocationView() override = default;

    void refresh();

protected:
    void updateSelf( uint32_t ticks, InputInterface &iinter, RenderInterface &rInter ) override;

private:
    Overworld* m_overworld;
    std::vector<std::shared_ptr<LocationViewItem>> m_locations;
    std::unordered_map<Vector2i, Sprite, Vector2iHash> m_connections;
};


}