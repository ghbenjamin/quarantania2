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

    void addConnectionLine( Vector2i startPos, Vector2i endPos, int width, int margins );


    Overworld* m_overworld;
    std::unordered_map<Vector2i, std::shared_ptr<LocationViewItem>, Vector2iHash> m_locations;
    std::unordered_map<Vector2i, Sprite, Vector2iHash> m_connections;
};


}