#include <ui/LevelUi.h>
#include <ui/Manager.h>
#include <game/Level.h>
#include <components/RenderComponent.h>

UI::TurnOrderWidget::TurnOrderWidget(UI::Manager *manager, UI::Element *parent, EntityRef ref)
: Element(manager, parent), m_entity(ref)
{
    setBorder( 2, Colour::Orange );
    setBackgroundColour( Colour::Olive );
    setPreferredContentSize({200, 0});
    setLayout<HorizontalLayout>( 1, VAlignment::Centre );


    auto iconSprite = manager->level()->getComponents<RenderComponent>(ref)->sprites[0];
    auto icon = manager->createElement<Icon>(this, iconSprite);
    icon->setPadding(2);

    auto labelText = manager->level()->getDescriptionForEnt(ref);
    m_nameLabel = manager->createElement<Label>(this);
    m_nameLabel->setText( std::string(labelText) );
}

void UI::TurnOrderWidget::refresh()
{

}

UI::TurnOrderContainer::TurnOrderContainer(UI::Manager *manager, UI::Element *parent)
: Element(manager, parent)
{
    setId("turn-order-container");
    setLayout<VerticalLayout>( 4, HAlignment::Left );
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
