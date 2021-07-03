#pragma once

#include <ui/lib/Element.h>
#include <engine/Entity.h>

class Level;

namespace UI
{

class Label;
class Icon;

class PlayerStatusActionSpeedBar : public Element
{
public:
    PlayerStatusActionSpeedBar( Manager* manager, Element* parent, Level* level, EntityRef entity );
    ~PlayerStatusActionSpeedBar() override = default;

    void refresh();

protected:
    void onAlphaModChange(float newValue) override;

private:
    Level* m_level;
    EntityRef m_entity;
    std::array<std::shared_ptr<Icon>, 3> m_icons;
    std::array<Sprite, 3> m_unusedSprites;
    std::array<Sprite, 3> m_usedSprites;
};


class PlayerStatusHP : public Element
{
public:
    PlayerStatusHP( Manager* manager, Element* parent, Level* level, EntityRef entity );
    ~PlayerStatusHP() override = default;
    
    void refresh();
    
private:
    Level* m_level;
    std::shared_ptr<Label> m_text;
    EntityRef m_entity;
};



class PlayerStatusWidget : public Element
{
public:
    PlayerStatusWidget(Manager* manager, Element* parent, Level* level, EntityRef entity );
    ~PlayerStatusWidget() override = default;
    
    void refresh();

private:
    
    void highlightEntity();
    void unhighlightEntity();
    void selectEntity();
    
    Level* m_level;
    EntityRef m_entity;
    std::shared_ptr<Label> m_nameLabel;
    std::shared_ptr<PlayerStatusActionSpeedBar> m_actionSpeed;
    std::shared_ptr<PlayerStatusHP> m_hp;
};


class PlayerStatusContainer : public Element
{
public:
    PlayerStatusContainer(Manager* manager, Element* parent, Level* level );
    ~PlayerStatusContainer() override = default;
    
    void reloadEntities();
    void refresh();

private:
    Level* m_level;
};

}