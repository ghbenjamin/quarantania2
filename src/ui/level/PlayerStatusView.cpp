#include <ui/level/PlayerStatusView.h>

#include <ui/lib/Manager.h>
#include <game/Level.h>
#include <game/Action.h>
#include <components/RenderComponent.h>
#include <components/PositionComponent.h>
#include <components/ActorComponent.h>


// Turn order Widget
// --------------------------------------

UI::PlayerStatusWidget::PlayerStatusWidget(UI::Manager *manager, UI::Element *parent, EntityRef ref)
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

void UI::PlayerStatusWidget::refresh()
{
    m_nameLabel->setColour(Colour::Black);
}

void UI::PlayerStatusWidget::highlightEntity()
{
    auto pos = manager()->level()->ecs().getComponents<PositionComponent>(m_entity)->tilePosition;
    manager()->level()->ui().showSingleTileHighlight(pos, UI::SingleTileHighlightType::Green);
}

void UI::PlayerStatusWidget::unhighlightEntity()
{
    manager()->level()->ui().removeSingleTileHighlight();
}

void UI::PlayerStatusWidget::selectEntity()
{

}




// Turn order widget container
// --------------------------------------


UI::PlayerStatusContainer::PlayerStatusContainer(UI::Manager *manager, UI::Element *parent)
        : Element(manager, parent)
{
    setId("turn-order-container");
    setLayout<VerticalLayout>( 4, HAlignment::Fill );
}

void UI::PlayerStatusContainer::refresh()
{
    for ( auto const& c : children() )
    {
        c->asType<PlayerStatusWidget>()->refresh();
    }
}

void UI::PlayerStatusContainer::reloadEntities()
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