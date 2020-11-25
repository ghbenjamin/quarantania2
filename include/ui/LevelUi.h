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



}