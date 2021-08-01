#pragma once

#include <ui/lib/Element.h>
#include <ui/lib/Controls.h>


class Overworld;
struct OverworldLocation;

namespace UI
{


class OverworldViewLocationItem : public Element
{
public:
    OverworldViewLocationItem( Manager *manager, Element *parent, Overworld* overworld, OverworldLocation const* loc, bool readOnly );

    void refresh();
    

private:
    void onClick();
    void onMouseIn();
    void onMouseOut();
    
    bool m_readOnly;
    OverworldLocation const* m_loc;
    Overworld* m_overworld;
    std::shared_ptr<Icon> m_icon;
};


class OverworldView : public Element
{
public:
    OverworldView( Manager *manager, Element *parent, Overworld* overworld, bool readOnly );
    ~OverworldView() override = default;

    void refresh();

protected:
    void updateSelf( uint32_t ticks, InputInterface &iinter, RenderInterface &rInter ) override;

private:

    void addConnectionLine( Vector2i startPos, Vector2i endPos, int width, int margins );


    Overworld* m_overworld;
    bool m_readOnly;
    std::vector<std::shared_ptr<OverworldViewLocationItem>> m_locations;
    std::unordered_map<Vector2i, Sprite, Vector2iHash> m_connections;
};


}