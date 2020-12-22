#include <ui/LevelUi.h>
#include <ui/Manager.h>
#include <game/Level.h>
#include <components/RenderComponent.h>
#include <components/ActorComponent.h>
#include <components/PositionComponent.h>
#include <resource/ResourceManager.h>
#include <game/LevelController.h>
#include <game/Action.h>

UI::TurnOrderWidget::TurnOrderWidget(UI::Manager *manager, UI::Element *parent, EntityRef ref)
: Element(manager, parent), m_entity(ref)
{
    // Layout
    setBorder( 2, Colour::White );
    setLayout<HorizontalLayout>( 1, VAlignment::Centre );

    // Colour the widget based on the type of actor
    auto actorC = manager->level()->ecs().getComponents<ActorComponent>(ref);
    if ( actorC->actorType == ActorType::PC )
    {
        setBackgroundColour( Colour(0, 153, 255, 220) );
    }
    else if (actorC->actorType == ActorType::NPC)
    {
        setBackgroundColour( Colour(255, 102, 102, 220) );
    }
    else
    {
        setBackgroundColour( Colour::Grey );
    }
    
    // Add the icon of the entity
    auto iconSprite = manager->level()->ecs().getComponents<RenderComponent>(ref)->sprites[0];
    auto icon = manager->createElement<Icon>(this, iconSprite);
    icon->setPadding(2);

    // Add the name of the entity
    auto labelText = manager->level()->getDescriptionForEnt(ref);
    m_nameLabel = manager->createElement<Label>(this);
    m_nameLabel->setText( std::string(labelText) );
    m_nameLabel->setPadding(4);

    // Let us select/hover the entity by selecting interacting with the widgets
    addEventCallback(UEventType::MouseIn, [this] (UEvent& evt) { this->highlightEntity(); });
    addEventCallback(UEventType::MouseOut, [this] (UEvent& evt) { this->unhighlightEntity(); });
    addEventCallback(UEventType::Click, [this] (UEvent& evt) { this->selectEntity(); });
}

void UI::TurnOrderWidget::refresh()
{
    if (manager()->level()->getActiveEntity() == m_entity)
    {
        m_nameLabel->setColour(Colour::White);
    }
    else
    {
        m_nameLabel->setColour(Colour::Black);
    }
}

void UI::TurnOrderWidget::highlightEntity()
{
    auto pos = manager()->level()->ecs().getComponents<PositionComponent>(m_entity)->tilePosition;
    manager()->level()->ui().showSingleTileHighlight(pos, UI::SingleTileHighlightType::Green);
}

void UI::TurnOrderWidget::unhighlightEntity()
{
    manager()->level()->ui().removeSingleTileHighlight();
}

void UI::TurnOrderWidget::selectEntity()
{

}

UI::TurnOrderContainer::TurnOrderContainer(UI::Manager *manager, UI::Element *parent)
: Element(manager, parent)
{
    setId("turn-order-container");
    setLayout<VerticalLayout>( 4, HAlignment::Fill );
}

void UI::TurnOrderContainer::refresh()
{
    for ( auto const& c : children() )
    {
        c->asType<TurnOrderWidget>()->refresh();
    }
}

void UI::TurnOrderContainer::reloadEntities()
{
    removeAllChildren();

    for ( auto const& entity : manager()->level()->turnOrder() )
    {
        auto actorComp = manager()->level()->ecs().getComponents<ActorComponent>(entity);
        if (actorComp->actorType == ActorType::PC)
        {
            manager()->createElement<TurnOrderWidget>(this, entity);
        }
    }

    refresh();
}





UI::ActionMenuPopupMenu::ActionMenuPopupMenu(UI::Manager *manager, UI::Element *parent,
     const std::vector<std::shared_ptr<Action>> &item,  RawActionDataType category)
    : Element(manager, parent), m_category(category)
{
    setId("action-menu-popup-menu");
    setLayout<VerticalLayout>( 4, HAlignment::Left );

    for ( auto const& act : item )
    {
        auto elem = manager->createElement<UI::Element>(this);
        elem->setPadding(4);
        elem->setBackgroundColour(Colour::Beige);
        elem->setLayout<HorizontalLayout>(8, VAlignment::Centre);

        manager->createElement<UI::Icon>(elem.get(), act->getSprite() );
        manager->createElement<UI::Label>(elem.get(), act->getName() );

        elem->addEventCallback(UEventType::Click, [manager, act, this](UEvent const& evt) {
            // Do something here!

            manager->level()->controller()->pushActionController( manager->level()->getActiveEntity(), act );

            manager->deleteElement( shared_from_this() );
        });
    }

    doLayout();
}

RawActionDataType UI::ActionMenuPopupMenu::getCategory() const
{
    return m_category;
}

UI::ActionMenuContainer::ActionMenuContainer(UI::Manager *manager, UI::Element *parent)
    : Element(manager, parent)
{
    setId("action-menu-container");
    setLayout<HorizontalLayout>( 8, VAlignment::Top );

    manager->createElement<UI::ActionMenuSpawnItem>(this, "Attack",
            SpritesheetKey{"game_icons", "axe-sword"}, RawActionDataType::Attack);
    manager->createElement<UI::ActionMenuSpawnItem>(this, "Move",
            SpritesheetKey{"game_icons", "move"}, RawActionDataType::Move);
    manager->createElement<UI::ActionMenuSpawnItem>(this, "Items",
            SpritesheetKey{"game_icons", "light-backpack"}, RawActionDataType::Item);
}

void UI::ActionMenuContainer::onSpawnItemHover(RawActionDataType category)
{
}

void UI::ActionMenuContainer::onSpawnItemClick(RawActionDataType category)
{
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

    std::vector<std::shared_ptr<Action>> menuItems;
    auto actions = manager()->level()->actionsForCurrentActor();

    for ( auto& action : actions )
    {
        if ( action->getType() == category )
        {
            menuItems.push_back( action );
        }
    }

    if ( menuItems.empty() )
    {
        return;
    }

    auto newMenu = manager()->createElement<UI::ActionMenuPopupMenu>(nullptr, std::move(menuItems), category );

    // Find the position of the button we clicked so that we can align new menu to it
    auto spawn = firstMatchingCondition( [category](ElementPtr const& elem) {
        return ( elem->hasTag("action-popup-spawn-icon") &&
            elem->parent()->asType<UI::ActionMenuSpawnItem>()->getCategory() == category );
    });

    Assert(!!spawn);

    Vector2i origin = globalPosition();

    origin.x( spawn->outerBounds().x() );
    origin.y( origin.y() - newMenu->outerBounds().h() );

    newMenu->setLocalPosition(origin);

    m_menu = newMenu;
}

UI::ActionMenuSpawnItem::ActionMenuSpawnItem(UI::Manager *manager, UI::Element *parent,
     std::string const& name, SpritesheetKey const& icon, RawActionDataType category)
    : Element(manager, parent), m_name(name), m_category(category)
{

    setLayout<VerticalLayout>( 4, HAlignment::Centre );


    auto iconElem = manager->createElement<UI::Icon>(this, icon);
    iconElem->setBackgroundColour(Colour::Grey);
    iconElem->setBorder(1, Colour::White);
    iconElem->addTag("action-popup-spawn-icon");

    auto textElem = manager->createElement<UI::Label>(this,
        TextStyle { Colour::White, ResourceManager::get().getDefaultFont(10) });
    textElem->setText(name);

    addEventCallback( UEventType::MouseIn, [this](UEvent& evt) {
        this->parent()->asType<UI::ActionMenuContainer>()->onSpawnItemHover(m_category);
    });

    addEventCallback( UEventType::Click, [this](UEvent& evt) {
        this->parent()->asType<UI::ActionMenuContainer>()->onSpawnItemClick(m_category);
    });
}

RawActionDataType UI::ActionMenuSpawnItem::getCategory() const
{
    return m_category;
}
