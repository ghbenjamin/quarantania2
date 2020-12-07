#pragma once

#include <ui/Element.h>
#include <engine/Entity.h>

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


//struct ActionMenuWidgetItem
//{
//    SpritesheetKey icon;
//    std::string label;
//    // callback?
//};
//
//using ActionMenuWidgetList = std::vector<ActionMenuWidgetItem>;
//
//class ActionMenuWidget : public Element
//{
//public:
//    ActionMenuWidget(Manager* manager, Element* parent, SpritesheetKey menuIcon, ActionMenuWidgetList const& contents);
//    ~ActionMenuWidget() override = default;
//private:
//};
//
//
//class ActionMenuContainer : public Element
//{
//public:
//    ActionMenuContainer(Manager* manager, Element* parent);
//    ~ActionMenuContainer() override = default;
//
//private:
//
//};


}