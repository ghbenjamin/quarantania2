#include <ui/LevelUi.h>
#include <ui/Manager.h>
#include <game/Level.h>
#include <components/RenderComponent.h>
#include <components/ActorComponent.h>
#include <components/PositionComponent.h>
#include <resource/ResourceManager.h>
#include <game/LevelController.h>
#include <game/Action.h>
#include <graphics/RenderInterface.h>



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
    setId("ui-equip-container");

    const Vector2i ChildSize = {164, 205};
    const Vector2i ParentSize = ChildSize + Vector2i {10, 10};

    setPreferredContentSize(ParentSize);

    setLayout<CenterLayout>();
    auto child = manager->createElement<UI::EquipUiInner>(this);

    child->setPreferredContentSize(ChildSize);
}

UI::EquipUiInner::EquipUiInner(UI::Manager *manager, UI::Element *parent) : Element(manager, parent)
{
    setId("ui-equip-inner");

    auto sprite = ResourceManager::get().getImageAsSprite( "equip-only" );
    setBackground( sprite );

    addRegion( CreatureEquipSlot::Head, "game_icons/pointy-hat", {24, 3} );
    addRegion( CreatureEquipSlot::Headband, "game_icons/headband-knot", { 66,3 } );
    addRegion( CreatureEquipSlot::Eyes, "game_icons/steampunk-goggles", { 108,3 } );

    addRegion( CreatureEquipSlot::Shoulders, "game_icons/pauldrons", { 24,45 });
    addRegion( CreatureEquipSlot::Neck, "game_icons/pearl-necklace", { 66,45 } );
    addRegion( CreatureEquipSlot::Chest, "game_icons/chest-armor", { 108,45 } );

    addRegion( CreatureEquipSlot::Belt, "game_icons/belt", { 24,87 } );
    addRegion( CreatureEquipSlot::Wrists, "game_icons/walking-boot", { 66,87 } );
    addRegion( CreatureEquipSlot::Arms, "game_icons/walking-boot", { 108,87 } );

    addRegion( CreatureEquipSlot::LeftHand, "game_icons/pointy-sword", { 3,129 } );
    addRegion( CreatureEquipSlot::Body, "game_icons/walking-boot", { 45,129 } );
    addRegion( CreatureEquipSlot::Armour, "game_icons/abdominal-armor", { 87,129 } );
    addRegion( CreatureEquipSlot::RightHand, "game_icons/pointy-sword", { 129,129 } );

    addRegion( CreatureEquipSlot::Ring1, "game_icons/ring",  { 24,170 } );
    addRegion( CreatureEquipSlot::Feet, "game_icons/leg-armor", { 66,170 } );
    addRegion( CreatureEquipSlot::Ring2, "game_icons/ring", { 108,170 } );


    addEventCallback(UEventType::MouseMove, [this] (UEvent& evt) {
        this->onMouseMove(evt.mouseMoveEvent);
    });

    addEventCallback(UEventType::Click, [this] (UEvent& evt) {
        this->onClick(evt.mouseButtonEvent);
    });

}

void UI::EquipUiInner::addRegion(CreatureEquipSlot slot, const SpritesheetKey &key, const Vector2i &offset)
{
    EquipSlotView view;
    view.sprite = ResourceManager::get().getSprite( key );
    view.offset = { offset, Vector2i{32, 32} };

    view.sprite.setRenderLayer( RenderLayer::UI );
    m_regions.emplace( slot, view );
}

void UI::EquipUiInner::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    auto offset = globalPosition();

    for (auto const& [k, v] : m_regions )
    {
        rInter.addScreenItem( v.sprite.renderObject( offset + v.offset.left() ) );
    }
}

void UI::EquipUiInner::onMouseMove(UMouseMoveEvent const& evt)
{

}

void UI::EquipUiInner::onClick(const UI::UMouseButtonEvent &evt)
{
    auto clicked = slotFromScreenPos( evt.pos );

    if (clicked)
    {
        if ( evt.button == UMouseButtonEvent::LeftMouseButton )
        {

        }
        else if ( evt.button == UMouseButtonEvent::RightMouseButton )
        {

        }
    }
}

std::optional<CreatureEquipSlot> UI::EquipUiInner::slotFromScreenPos(Vector2i pos) const
{
    auto localPos = pos - globalPosition() - contentOffset();

    for (auto const& [k, v] : m_regions )
    {
        if ( v.offset.contains(localPos) )
        {
            return k;
        }
    }

    return {};
}
