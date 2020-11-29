#include <ui/LevelUi.h>
#include <ui/Manager.h>
#include <game/Level.h>
#include <components/RenderComponent.h>
#include <components/ActorComponent.h>

UI::TurnOrderWidget::TurnOrderWidget(UI::Manager *manager, UI::Element *parent, EntityRef ref)
: Element(manager, parent), m_entity(ref)
{
    setBorder( 2, Colour::White );
    setLayout<HorizontalLayout>( 1, VAlignment::Centre );

    auto actorC = manager->level()->getComponents<ActorComponent>(ref);
    if ( actorC->actorType == ActorType::PC )
    {
        setBackgroundColour( Colour::Green );
    }
    else if (actorC->actorType == ActorType::NPC)
    {
        setBackgroundColour( Colour::Red );
    }
    else
    {
        setBackgroundColour( Colour::Grey );
    }
    

    auto iconSprite = manager->level()->getComponents<RenderComponent>(ref)->sprites[0];
    auto icon = manager->createElement<Icon>(this, iconSprite);
    icon->setPadding(2);

    auto labelText = manager->level()->getDescriptionForEnt(ref);
    m_nameLabel = manager->createElement<Label>(this);
    m_nameLabel->setText( std::string(labelText) );
    m_nameLabel->setPadding(4);
}

void UI::TurnOrderWidget::refresh()
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
        manager()->createElement<TurnOrderWidget>(this, entity);
    }

    refresh();
}