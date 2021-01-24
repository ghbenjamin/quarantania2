#include <ui/LevelUi.h>
#include <ui/Manager.h>
#include <game/Level.h>
#include <components/RenderComponent.h>
#include <components/ActorComponent.h>
#include <components/PositionComponent.h>
#include <resource/ResourceManager.h>
#include <game/LevelController.h>
#include <game/Action.h>



// Turn order Widget
// --------------------------------------

UI::TurnOrderWidget::TurnOrderWidget(UI::Manager *manager, UI::Element *parent, EntityRef ref)
: Element(manager, parent), m_entity(ref)
{
    // Layout
    setBorder( Colour::Grey, 2, Colour::White );
    setLayout<HorizontalLayout>( 1, VAlignment::Centre );

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



// Action menu
// --------------------------------------

UI::ActionMenuPopupMenu::ActionMenuPopupMenu(UI::Manager *manager, UI::Element *parent,
     std::vector<std::shared_ptr<GameAction>> const& items,  RawActionDataType category)
    : Element(manager, parent), m_category(category)
{
    setId("action-menu-popup-menu");
    setLayout<VerticalLayout>( 4, HAlignment::Left );

    for ( std::shared_ptr <GameAction> const& act : items )
    {
        auto elem = manager->createElement<UI::Element>(this);
        elem->setPadding(4);
        elem->setBackground(Colour::Beige);
        elem->setLayout<HorizontalLayout>(8, VAlignment::Centre);

        manager->createElement<UI::Icon>(elem.get(), act->data.getSprite() );
        manager->createElement<UI::Label>(elem.get(), act->data.getName() );

        elem->addEventCallback(UEventType::Click, [manager, act, this](UEvent const& evt) {

            auto actionMenu = manager->firstElementMatching(
                [](ElementPtr const& elem){ return elem->id() == "action-menu"; }
            )->asType<ActionMenu>();

            manager->level()->controller()->pushActionController( actionMenu->currentEntity(), act );
            manager->deleteElement( shared_from_this() );
        });
    }

    doLayout();
}

RawActionDataType UI::ActionMenuPopupMenu::getCategory() const
{
    return m_category;
}

UI::ActionMenu::ActionMenu(UI::Manager *manager, UI::Element *parent)
    : Element(manager, parent), m_currEntity(EntityNull)
{
    setId("action-menu");
    setLayout<HorizontalLayout>( 8, VAlignment::Top );

    m_spawns[RawActionDataType::Attack] = manager->createElement<UI::ActionMenuSpawnItem>(this, "Attack",
            SpritesheetKey{"game_icons", "axe-sword"}, RawActionDataType::Attack);
    m_spawns[RawActionDataType::Move] = manager->createElement<UI::ActionMenuSpawnItem>(this, "Move",
            SpritesheetKey{"game_icons", "move"}, RawActionDataType::Move);
    m_spawns[RawActionDataType::Item] = manager->createElement<UI::ActionMenuSpawnItem>(this, "Items",
            SpritesheetKey{"game_icons", "light-backpack"}, RawActionDataType::Item);
}

void UI::ActionMenu::onSpawnItemMouseIn(RawActionDataType category)
{
    ResourceManager::get().getWindow()->cursor().setCursorType( CursorType::Interact );
}

void UI::ActionMenu::onSpawnItemMouseOut(RawActionDataType category)
{
    ResourceManager::get().getWindow()->cursor().resetCursor();
}

void UI::ActionMenu::onSpawnItemClick(RawActionDataType category)
{
    toggleMenu(category);
}

void UI::ActionMenu::refresh(EntityRef entity)
{
    m_currEntity = entity;

    bool isPC = false;

    if (manager()->level()->ecs().entityHas<ActorComponent>(entity))
    {
        isPC = manager()->level()->ecs().getComponents<ActorComponent>(entity)->actorType == ActorType::PC;
    }

    // If there's no
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
    std::vector<std::shared_ptr<GameAction>> menuItems;
    auto actions = manager()->level()->actionsForActor(m_currEntity);

    for ( auto& action : actions )
    {
        if ( action->data.getType() == category )
        {
            menuItems.push_back( std::move(action) );
        }
    }

    if ( menuItems.empty() )
    {
        return;
    }

    auto newMenu = manager()->createElement<UI::ActionMenuPopupMenu>(nullptr, menuItems, category);

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



UI::ActionMenuSpawnItem::ActionMenuSpawnItem(UI::Manager *manager, UI::Element *parent,
     std::string const& name, SpritesheetKey const& icon, RawActionDataType category)
    : Element(manager, parent), m_name(name), m_category(category), m_isDisabled(true)
{
    setLayout<VerticalLayout>( 4, HAlignment::Centre );

    m_icon = manager->createElement<UI::Icon>(this, icon);
    m_icon->setBorder( Colour::Grey.withAlpha(150), 1, Colour::White.withAlpha(150) );
    m_icon->addTag("action-popup-spawn-icon");

    m_label = manager->createElement<UI::Label>(this,
        TextStyle { Colour::White.withAlpha(150), ResourceManager::get().getDefaultFont(10) });
    m_label->setText(name);

    addEventCallback( UEventType::MouseIn, [this](UEvent& evt) {
        if (!m_isDisabled)
        {
            this->parent()->asType<UI::ActionMenu>()->onSpawnItemMouseIn(m_category);
        }
    });

    addEventCallback( UEventType::MouseOut, [this](UEvent& evt) {
        if (!m_isDisabled)
        {
            this->parent()->asType<UI::ActionMenu>()->onSpawnItemMouseOut(m_category);
        }
    });

    addEventCallback( UEventType::Click, [this](UEvent& evt) {
        if (!m_isDisabled)
        {
            this->parent()->asType<UI::ActionMenu>()->onSpawnItemClick(m_category);
        }
    });
}

RawActionDataType UI::ActionMenuSpawnItem::getCategory() const
{
    return m_category;
}

void UI::ActionMenuSpawnItem::setDisabled(bool value)
{
    m_isDisabled = value;

    if (m_isDisabled)
    {
        m_icon->setBorder( Colour::Grey.withAlpha(150), 1, Colour::White.withAlpha(150) );
        m_label->setColour( Colour::White.withAlpha(150) );
    }
    else
    {
        m_icon->setBorder( Colour::Grey, 1, Colour::White );
        m_label->setColour( Colour::White );
    }
}

bool UI::ActionMenuSpawnItem::isDisabled() const
{
    return m_isDisabled;
}



// Description container
// --------------------------------------


UI::HoverDescriptionContainer::HoverDescriptionContainer(UI::Manager *manager, UI::Element *parent)
    : Element(manager, parent)
{
    setId("hover-desc-container");
    setLayout<VerticalLayout>( 0, HAlignment::Left );

    const int TotalWidth = 300;
    const int TotalHeight = 120;
    const int TitleHeight = 30;
    const int PaddingW = 4;
    const int PaddingWx2 = 2 * PaddingW;

    setPreferredContentSize({TotalWidth - PaddingWx2, TotalHeight - PaddingWx2});
    setBackground( Colour::Grey.withAlpha(200) );
    setPadding(PaddingW);

    auto titleContainer = manager->createElement(this);
    titleContainer->setPreferredContentSize({TotalWidth - PaddingWx2, TitleHeight});
    titleContainer->setLayout<VerticalLayout>( 0, HAlignment::Left );

    m_titleElem = manager->createElement<Label>(
        titleContainer.get(),
        TextStyle{Colour::Black, ResourceManager::get().getDefaultFont(16)}
    );

    auto descContainer = manager->createElement(this);
    descContainer->setPreferredContentSize({TotalWidth - PaddingWx2, TotalHeight - TitleHeight - PaddingWx2});
    descContainer->setLayout<VerticalLayout>( 0, HAlignment::Left );

    m_descElem = manager->createElement<Label>(
        descContainer.get(),
        TextStyle{Colour::Black, ResourceManager::get().getDefaultFont(12)}
    );

    m_descElem->setMaximumOuterSize( {TotalWidth - PaddingWx2, TotalHeight - TitleHeight - PaddingWx2});
}

void UI::HoverDescriptionContainer::setData( std::string const& data, std::string const& desc )
{
    m_titleElem->setText( data );
    m_descElem->setText( desc );
}

void UI::HoverDescriptionContainer::clearData()
{
    m_titleElem->clearText();
    m_descElem->clearText();
}


UI::EquipUi::EquipUi(UI::Manager *manager, UI::Element *parent) : Element(manager, parent)
{
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border" );
    NinePatch np = { patch.texture(), patch.offsets() };

    setBackground( np );
    setBorderWidth( patch.borderWidth() );

//    setBackground( Colour::Grey );
    setPreferredContentSize({200, 300});

    setLayout<FreeLayout>();
    manager->createElement<UI::EquipUiInner>(this);
}

UI::EquipUiInner::EquipUiInner(UI::Manager *manager, UI::Element *parent) : Element(manager, parent)
{
    std::vector<Vector2i> locs = {
            { 24,3 },
            { 66,3 },
            { 108,3 },
            { 24,45 },
            { 66,45 },
            { 108,45 },
            { 24,87 },
            { 66,87 },
            { 108,87 },
            { 3,129 },
            { 45,129 },
            { 87,129 },
            { 129,129 },
            { 24,170 },
            { 66,170 },
            { 108,170 },
    };

    setPreferredContentSize({164, 205});
    auto sprite = ResourceManager::get().getImageAsSprite( "equip-only" );
    setBackground( sprite );

}
