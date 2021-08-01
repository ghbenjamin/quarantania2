#include <ui/level/ActionPopupMenu.h>

#include <ui/lib/Manager.h>
#include <game/Level.h>
#include <game/Action.h>
#include <resource/ResourceManager.h>
#include <components/ActorComponent.h>
#include <utils/Memory.h>
#include <ui/lib/Tooltips.h>


// Popup menu
// --------------------------------------

UI::ActionMenuPopupMenu::ActionMenuPopupMenu(UI::Manager *manager, UI::Element *parent, Level* level,
                                             std::vector<GameAction> const& items,  RawActionDataType category)
        : Element(manager, parent), m_category(category), m_level(level)
{
    setId("action-menu-popup-menu");
    setLayout<VerticalLayout>( 4, HAlignment::Left );
    
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border-solid" ).getPatch();
    
    for ( GameAction const& act : items )
    {
        auto elem = manager->createElement<UI::Element>(this);
        elem->setPadding(2);
        elem->setPreferredContentSize({150, 10});
        elem->setLayout<HorizontalLayout>(8, VAlignment::Centre);

        auto iconElem = manager->createElement<UI::Icon>(elem.get(), act.data.sprite );
        iconElem->getSprite().setPermanentColour( Colour::Black );
        iconElem->setPadding(2);
        
        auto labelElem = manager->createElement<UI::Label>(elem.get(), act.data.name );
        labelElem->setTextColour(Colour::White);

        if (act.enabled)
        {
            elem->setBackground(patch);
            elem->addEventCallback(UEventType::Click, [manager, act, this](UEvent const& evt) {
            
                auto actionMenu = manager->firstElementMatching(
                         [](ElementPtr const& elem){ return elem->id() == "action-menu"; }
                )->asType<ActionMenu>();

                // TODO FIX CONTROLLER ACTIONS BROKEN ARGH
//                m_level->controller()->pushActionController( actionMenu->currentEntity(), act );
                manager->deleteElement( shared_from_this() );
            });
        }
        else
        {
            elem->setBackground(Colour::Grey);
        }
        
        elem->setTooltipSpawner( [=] () {
            return manager->createElement<SimpleTooltip>(nullptr, act.data.name, act.data.description);
        });
    }
    
    doLayout();
}

RawActionDataType UI::ActionMenuPopupMenu::getCategory() const
{
    return m_category;
}



// Menu bar
// --------------------------------------

UI::ActionMenu::ActionMenu(UI::Manager *manager, UI::Element *parent, Level* level)
        : Element(manager, parent), m_currEntity(EntityNull), m_level(level)
{
    setId("action-menu");
    setLayout<HorizontalLayout>( 4, VAlignment::Top );
    
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border-solid" ).getPatch();
    setBackground( patch );
    setBorderWidth( patch.getBorderWidth() );
    
    m_spawns[RawActionDataType::Attack] = manager->createElement<UI::ActionMenuSpawnItem>(
        this, level, "Attack", "game_ui/w-axe-sword", RawActionDataType::Attack);
    m_spawns[RawActionDataType::Move] = manager->createElement<UI::ActionMenuSpawnItem>(
        this, level, "Move", "game_ui/w-move", RawActionDataType::Move);
    m_spawns[RawActionDataType::Item] = manager->createElement<UI::ActionMenuSpawnItem>(
        this, level, "Items", "game_ui/w-light-backpack", RawActionDataType::Item);
}

void UI::ActionMenu::onSpawnItemClick(RawActionDataType category)
{
    toggleMenu(category);
}

void UI::ActionMenu::refresh(EntityRef entity)
{
    m_currEntity = entity;
    
    bool isPC = false;
    
    if (m_level->ecs().entityHas<ActorComponent>(entity))
    {
        isPC = m_level->ecs().getComponents<ActorComponent>(entity)->actorType == ActorType::PC;
    }
    
    // If there's no selected player entity, disable all buttons
    if ((m_currEntity == EntityNull) || !isPC)
    {
        for (auto const& [k, v] : m_spawns)
        {
            v->setDisabled(true);
        }
    }
    else
    {
        for (auto const& [k, v] : m_spawns)
        {
            v->setDisabled(false);
        }
    }
}

EntityRef UI::ActionMenu::currentEntity() const
{
    return m_currEntity;
}

void UI::ActionMenu::openMenu(RawActionDataType category)
{
    std::vector<GameAction> menuItems;
    auto actions = m_level->actionsForActor(m_currEntity);
    
    for ( auto& action : actions )
    {
        if ( action.data.type == category )
        {
            menuItems.push_back( std::move(action) );
        }
    }
    
    if ( menuItems.empty() )
    {
        return;
    }
    
    auto newMenu = manager()->createElement<UI::ActionMenuPopupMenu>(nullptr, m_level, menuItems, category);
    
    // Find the position of the button we clicked so that we can align new menu to it
    auto spawn = firstDescMatchingCondition([category]( ElementPtr const &elem ) {
        return (elem->hasTag("action-popup-spawn-icon") &&
                elem->asType<UI::ActionMenuSpawnItem>()->getCategory() == category);
    });
    
    Assert(!!spawn);
    
    Vector2i origin = globalPosition();
    
    origin.x( spawn->outerBounds().x() );
    origin.y( origin.y() - newMenu->outerBounds().h() );
    
    origin = origin + Vector2i(4, -4);
    
    newMenu->setLocalPosition(origin);
    
    m_menu = newMenu;
}

void UI::ActionMenu::toggleMenu(RawActionDataType category)
{
    if (m_spawns.at(category)->isDisabled())
    {
        return;
    }
    
    auto menu = m_menu.lock();
    
    if ( menu )
    {
        auto existing = menu->getCategory();
        
        manager()->deleteElement( menu );
        m_menu.reset();
        
        if (existing == category)
        {
            return;
        }
    }
    
    openMenu(category);
}

void UI::ActionMenu::closeMenu()
{
    auto menu = m_menu.lock();
    
    if ( menu )
    {
        manager()->deleteElement( menu );
        m_menu.reset();
    }
}


// Menu bar item
// --------------------------------------

UI::ActionMenuSpawnItem::ActionMenuSpawnItem(UI::Manager *manager, UI::Element *parent, Level* level,
 std::string const& desc, SpritesheetKey const& icon, RawActionDataType category)
        : IconButton(manager, parent, icon, [this](){ onClick(); }), m_category(category), m_level(level)
{
    addTag("action-popup-spawn-icon");
    setDisabled(true);
}

RawActionDataType UI::ActionMenuSpawnItem::getCategory() const
{
    return m_category;
}

void UI::ActionMenuSpawnItem::onClick()
{
    this->parent()->asType<UI::ActionMenu>()->onSpawnItemClick(m_category);
}