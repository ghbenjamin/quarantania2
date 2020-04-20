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

    void reimportItems();

private:
    std::shared_ptr<TextNode> m_nameNode;
    std::shared_ptr<TextNode> m_classNode;

    std::vector<std::shared_ptr<TextNode>> m_abilityScoreHeaders;
    std::vector<std::shared_ptr<TextNode>> m_abilityScoreValues;
    std::vector<std::shared_ptr<TextNode>> m_abilityScoreModifiers;
};
}

