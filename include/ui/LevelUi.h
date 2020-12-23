#pragma once

#include <ui/Element.h>
#include <engine/Entity.h>
#include <game/RawData.h>

class Action;
struct GameAction;

namespace UI
{

class Label;
class Icon;

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
    ActionMenuPopupMenu(Manager* manager, Element* parent, std::vector<std::shared_ptr<GameAction>> const& item, RawActionDataType category);
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
    void setDisabled(bool value);
    bool isDisabled() const;

private:
    std::string m_name;
    RawActionDataType m_category;
    bool m_isDisabled;

    std::shared_ptr<Icon> m_icon;
    std::shared_ptr<Label> m_label;
};


class ActionMenu : public Element
{
public:
    ActionMenu(Manager* manager, Element* parent);
    ~ActionMenu() override = default;

    void onSpawnItemHover( RawActionDataType category );
    void onSpawnItemClick( RawActionDataType category );

    void refresh(EntityRef entity);
    EntityRef currentEntity() const;

    void toggleMenu( RawActionDataType category );

    void closeMenu();
    void openMenu( RawActionDataType category );

private:


    EntityRef m_currEntity;
    std::weak_ptr<ActionMenuPopupMenu> m_menu;
    std::unordered_map<RawActionDataType, std::shared_ptr<ActionMenuSpawnItem>> m_spawns;
};


}