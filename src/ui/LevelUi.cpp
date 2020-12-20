#include <ui/LevelUi.h>
#include <ui/Manager.h>
#include <game/Level.h>
#include <components/RenderComponent.h>
#include <components/ActorComponent.h>
#include <components/PositionComponent.h>
#include <resource/ResourceManager.h>

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
     const std::vector<ActionMenuItem> &item)
    : Element(manager, parent)
{

}

UI::ActionMenuContainer::ActionMenuContainer(UI::Manager *manager, UI::Element *parent)
    : Element(manager, parent)
{
    setId("action-menu-container");
    setLayout<HorizontalLayout>( 8, VAlignment::Top );

    manager->createElement<UI::ActionMenuSpawnItem>(this, "Combat",
            SpritesheetKey{"game_icons", "axe-sword"}, RawActionDataType::Attack);
    manager->createElement<UI::ActionMenuSpawnItem>(this, "Movement",
            SpritesheetKey{"game_icons", "move"}, RawActionDataType::Move);
    manager->createElement<UI::ActionMenuSpawnItem>(this, "Inventory",
            SpritesheetKey{"game_icons", "light-backpack"}, RawActionDataType::Item);
}

void UI::ActionMenuContainer::onSpawnItemHover(RawActionDataType category)
{
    Logging::log( (int) category );
}

UI::ActionMenuSpawnItem::ActionMenuSpawnItem(UI::Manager *manager, UI::Element *parent,
     std::string const& name, SpritesheetKey icon, RawActionDataType category)
    : Element(manager, parent), m_name(name), m_category(category)
{
    manager->createElement<UI::Icon>(this, icon);
    setBackgroundColour(Colour::Grey);
    setBorder(1, Colour::White);

    addEventCallback( UEventType::MouseIn, [this](UEvent& evt) {
        this->parent()->asType<UI::ActionMenuContainer>()->onSpawnItemHover(m_category);
    });
}
