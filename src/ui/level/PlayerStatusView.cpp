#include <ui/level/PlayerStatusView.h>

#include <ui/lib/Manager.h>
#include <game/Level.h>
#include <game/Action.h>
#include <components/RenderComponent.h>
#include <components/PositionComponent.h>
#include <components/ActorComponent.h>
#include <resource/ResourceManager.h>


using namespace UI; 

// Turn order Widget
// --------------------------------------

PlayerStatusWidget::PlayerStatusWidget(Manager *manager, Element *parent, EntityRef ref)
        : Element(manager, parent), m_entity(ref)
{
    // Layout
    setLayout<HorizontalLayout>( 1, VAlignment::Centre );
    setPreferredContentWidth( 200 );
    
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border" );
    NinePatch np = { patch.texture(), patch.offsets() };
    
    setBackground( np );
    setBorderWidth( patch.borderWidth() );
    
    // Add the icon of the entity
    auto& iconSprite = manager->level()->ecs().getComponents<RenderComponent>(ref)->sprite;
    auto icon = manager->createElement<Icon>(this, iconSprite);
    icon->setPadding(2);
    
    auto rightCont = manager->createElement(this);
    rightCont->setLayout<VerticalLayout>(0, HAlignment::Left );
    rightCont->setPadding(4);
    rightCont->setPreferredContentWidth(150);
    
    // Add the name of the entity
    auto labelText = manager->level()->getDescriptionForEnt(ref);
    m_nameLabel = manager->createElement<Label>( rightCont.get() );
    m_nameLabel->setText( std::string(labelText) );
    m_nameLabel->setPadding(2);
    
    auto speedHPcont = manager->createElement( rightCont.get() );
    speedHPcont->setLayout<HorizontalSpaceBetweenLayout>( VAlignment::Centre );
    speedHPcont->setPreferredContentWidth(150);

    m_actionSpeed = manager->createElement<PlayerStatusActionSpeedBar>( speedHPcont.get(), ref );
    m_hp = manager->createElement<PlayerStatusHP>( speedHPcont.get(), ref );

    // Let us select/hover the entity by selecting interacting with the widgets
    addEventCallback(UEventType::MouseIn, [this] (UEvent& evt) { this->highlightEntity(); });
    addEventCallback(UEventType::MouseOut, [this] (UEvent& evt) { this->unhighlightEntity(); });
    addEventCallback(UEventType::Click, [this] (UEvent& evt) { this->selectEntity(); });
}

void PlayerStatusWidget::refresh()
{
    m_nameLabel->setTextColour(Colour::Black);
    m_actionSpeed->refresh();
    m_hp->refresh();
}

void PlayerStatusWidget::highlightEntity()
{
    auto pos = manager()->level()->ecs().getComponents<PositionComponent>(m_entity)->tilePosition;
    manager()->level()->ui().showSingleTileHighlight(pos, SingleTileHighlightType::Green);
}

void PlayerStatusWidget::unhighlightEntity()
{
    manager()->level()->ui().removeSingleTileHighlight();
}

void PlayerStatusWidget::selectEntity()
{

}




// Turn order widget container
// --------------------------------------


PlayerStatusContainer::PlayerStatusContainer(Manager *manager, Element *parent)
        : Element(manager, parent)
{
    setId("player-status-container");
    setLayout<VerticalLayout>( 4, HAlignment::Fill );
}

void PlayerStatusContainer::refresh()
{
    for ( auto const& c : children() )
    {
        c->asType<PlayerStatusWidget>()->refresh();
    }
}

void PlayerStatusContainer::reloadEntities()
{
    removeAllChildren();

    auto ents = manager()->level()->ecs().entitiesHaving<ActorComponent>();
    std::sort( ents.begin(), ents.end() );

    for (auto const& ref : ents )
    {
        auto actorC = manager()->level()->ecs().getComponents<ActorComponent>(ref);

        if (actorC->actorType == ActorType::PC)
        {
            manager()->createElement<PlayerStatusWidget>(this, ref);
        }
    }
    
    refresh();
}


// Action speed bar
// --------------------------------------


PlayerStatusActionSpeedBar::PlayerStatusActionSpeedBar( Manager *manager, Element *parent, EntityRef entity )
  : Element(manager, parent), m_entity(entity)
{
    setLayout<HorizontalLayout>( -1, VAlignment::Centre );

   m_usedSprites = {
        ResourceManager::get().getSprite( "game_ui/action-std-disabled" ),
        ResourceManager::get().getSprite( "game_ui/action-move-disabled" ),
        ResourceManager::get().getSprite( "game_ui/action-swift-disabled" ),
    };
    
    m_unusedSprites = {
        ResourceManager::get().getSprite( "game_ui/action-std-enabled" ),
        ResourceManager::get().getSprite( "game_ui/action-move-enabled" ),
        ResourceManager::get().getSprite( "game_ui/action-swift-enabled" ),
    };


    m_icons = {
        manager->createElement<Icon>( this, m_unusedSprites[0] ),
        manager->createElement<Icon>( this, m_unusedSprites[1]  ),
        manager->createElement<Icon>( this, m_unusedSprites[2]  ),
    };
}

void PlayerStatusActionSpeedBar::refresh()
{
    auto& actorC = manager()->level()->ecs().getComponents<ActorComponent>(m_entity)->actor;
    auto used = actorC.actionInfo().getUsedActions();
    
    for (int i = 0; i < 3; i++)
    {
        if (!used[i])
        {
            m_icons[i]->setSprite( m_unusedSprites[i] );
        }
        else
        {
            m_icons[i]->setSprite( m_usedSprites[i] );
        }
    }
}

void PlayerStatusActionSpeedBar::onAlphaModChange(float newValue)
{
    for (auto & s: m_usedSprites)
    {
        s.setAlphaMod( newValue );
    }
    
    for (auto & s: m_unusedSprites)
    {
        s.setAlphaMod( newValue );
    }
}

PlayerStatusHP::PlayerStatusHP( Manager *manager, Element *parent, EntityRef entity )
    : Element(manager, parent), m_entity(entity)
{
    m_text = manager->createElement<Label>( this );
}

void PlayerStatusHP::refresh()
{
    auto& actorC = manager()->level()->ecs().getComponents<ActorComponent>(m_entity)->actor;
    
    m_text->setText( fmt::format(
        "{}/{}",
        actorC.getCurrentHp(),
        actorC.getMaxHp()
    ));
    
    setPreferredContentSize( m_text->outerBounds().right() );
}
