#pragma once

#include <ui/Element.h>
#include <game/Entity.h>

namespace UI
{

class TextNode;

class CharStatsView : public Element
{

public:
    CharStatsView(Manager *manager, Element *parent);
    ~CharStatsView() override = default;

    void attachEntity(EntityRef entity);
    void reimportItems();

private:

    EntityRef m_entity;

    std::shared_ptr<TextNode> m_nameNode;
    std::shared_ptr<TextNode> m_classNode;


};
}

