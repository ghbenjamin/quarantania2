#pragma once

#include <ui/Element.h>
#include <engine/Entity.h>
#include <game/RawData.h>

namespace UI
{

class Label;
class Action;

class TurnOrderWidget : public Element
{
public:
    TurnOrderWidget( Manager* manager, Element* parent, EntityRef entity );
    ~TurnOrderWidget() override = default;

    void refresh();

private:

    void highlightEntity();
    void unhighlightEntity();
    void selectEntity();


    EntityRef m_entity;
    std::shared_ptr<Label> m_nameLabel;
};


class TurnOrderContainer : public Element
{
public:
    TurnOrderContainer( Manager* manager, Element* parent );
    ~TurnOrderContainer() override = default;

    void reloadEntities();
    void refresh();
};



struct ActionMenuItem
{
    SpritesheetKey icon;
    std::string name;
    std::string description;
    bool enabled;
};


class ActionMenuPopupMenu : public Element
{
public:
    ActionMenuPopupMenu(Manager* manager, Element* parent, std::vector<ActionMenuItem> const& item);
    ~ActionMenuPopupMenu() override = default;
};


class ActionMenuSpawnItem : public Element
{
public:
    ActionMenuSpawnItem(Manager* manager, Element* parent, std::string const& name,
            SpritesheetKey icon, RawActionDataType category);
    ~ActionMenuSpawnItem() override = default;

private:
    std::string m_name;
    RawActionDataType m_category;
};


class ActionMenuContainer : public Element
{
public:
    ActionMenuContainer(Manager* manager, Element* parent);
    ~ActionMenuContainer() override = default;

    void onSpawnItemHover( RawActionDataType category );
};


}