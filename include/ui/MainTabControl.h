#pragma once

#include <ui/Element.h>

namespace UI
{

enum class MainTabSection
{
    InventoryEquip,
    CharacterStats,
    SkillsFeats,
    Spells,
};


class TabControlButton : public Element
{
public:
    TabControlButton(Manager *manager, Element *parent, int idx);
    ~TabControlButton() = default;

    int index() const;

private:

    int m_index;
};



class MainTabControl : public Element
{

public:
    MainTabControl(Manager* manager, Element* parent);
    ~MainTabControl() override = default;

private:

    ElementPtr addTab();
    void setCurrentTab( int idx );

private:

    static const int TOTAL_WIDTH = 400;
    static const int TOTAL_HEIGHT = 500;
    static const int TAB_BUTTONS_WIDTH = 50;
    static const int TAB_BUTTONS_HEIGHT = 80;

    std::vector<ElementPtr> m_tabContents;
    ElementPtr m_buttonHolder;
    ElementPtr m_contentHolder;
    int m_currentIdx;
};

}

