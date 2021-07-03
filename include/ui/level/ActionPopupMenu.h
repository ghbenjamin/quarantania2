#pragma once

#include <ui/lib/Element.h>
#include <ui/lib/Controls.h>
#include <game/RawData.h>
#include <engine/Entity.h>


class Level;
struct GameAction;

namespace UI
{


class ActionMenuPopupMenu : public Element
{
public:
    ActionMenuPopupMenu(Manager* manager, Element* parent, Level* level, std::vector<GameAction> const& item, RawActionDataType category);
    ~ActionMenuPopupMenu() override = default;
    
    RawActionDataType getCategory() const;

private:
    RawActionDataType m_category;
    Level* m_level;
};


class ActionMenuSpawnItem : public IconButton
{
public:
    ActionMenuSpawnItem(Manager* manager, Element* parent, Level* level, std::string const& desc, SpritesheetKey const& icon, RawActionDataType category);
    ~ActionMenuSpawnItem() override = default;
    
    RawActionDataType getCategory() const;

private:
    void onClick();
    
    RawActionDataType m_category;
    Level* m_level;
};


class ActionMenu : public Element
{
public:
    ActionMenu(Manager* manager, Element* parent, Level* level);
    ~ActionMenu() override = default;
    
    void onSpawnItemClick( RawActionDataType category );
    
    void refresh(EntityRef entity);
    EntityRef currentEntity() const;
    
    void toggleMenu( RawActionDataType category );
    
    void closeMenu();
    void openMenu( RawActionDataType category );

private:
    Level* m_level;
    EntityRef m_currEntity;
    std::weak_ptr<ActionMenuPopupMenu> m_menu;
    std::unordered_map<RawActionDataType, std::shared_ptr<ActionMenuSpawnItem>> m_spawns;
};



}