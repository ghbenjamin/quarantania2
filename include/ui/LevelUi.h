#pragma once

#include <ui/Element.h>
#include <engine/Entity.h>
#include <game/RawData.h>

class Action;
struct GameAction;

namespace UI
{

class Label;

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

class ActionMenuPopupMenu : public Element
{
public:
    ActionMenuPopupMenu(Manager* manager, Element* parent, std::vector<std::shared_ptr<GameAction>>& item, RawActionDataType category);
    ~ActionMenuPopupMenu() override = default;

    RawActionDataType getCategory() const;

private:
    RawActionDataType m_category;
};


class ActionMenuSpawnItem : public Element
{
public:
    ActionMenuSpawnItem(Manager* manager, Element* parent, std::string const& name,
            SpritesheetKey const& icon, RawActionDataType category);
    ~ActionMenuSpawnItem() override = default;

    RawActionDataType getCategory() const;

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
    void onSpawnItemClick( RawActionDataType category );

private:
    std::weak_ptr<ActionMenuPopupMenu> m_menu;
};


}