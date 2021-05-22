#include <ui/level/EntityInformationView.h>
#include <ui/lib/Manager.h>
#include <resource/ResourceManager.h>
#include <game/Level.h>
#include <components/ActorComponent.h>

using namespace UI;

UI::EntityInformationView::EntityInformationView( UI::Manager *manager, UI::Element *parent )
    : Element(manager, parent)
{
    setId("entity-information-view");
    
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border" ).getPatch();
    setBackground( patch );
    setBorderWidth( patch.getBorderWidth() );
    setLayout<VerticalLayout>( 2, HAlignment::Centre );
    
    setPreferredContentSize({150, 10});
    
    m_titleLabel = manager->createElement<Label>(this);
    m_titleLabel->setTextSize(16);
    m_titleLabel->setTextColour(Colour::White);
    m_titleLabel->setPadding(4);
}

void UI::EntityInformationView::refresh( EntityRef entity )
{
    if (entity == EntityNull)
    {
        return;
    }
    
    m_titleLabel->setText(manager()->level()->getDescriptionForEnt( entity ));
    
    for ( auto const& elem : m_dividers )
    {
        manager()->deleteElement( elem );
    }
    
    for ( auto const& elem : m_sections )
    {
        manager()->deleteElement( elem );
    }
    
    auto actorC = manager()->level()->ecs().tryGetComponent<ActorComponent>( entity );
    if ( actorC )
    {
        if (actorC->actorType == ActorType::PC )
        {
            auto s1 = addSection();
            s1->setText("holla!");
        }
        else
        {
            auto s1 = addSection();
            s1->setText("Foo");
            
            auto s2 = addSection();
            s2->setText("Bar");
        }
    }
    
    else
    {
    
    }
}

std::shared_ptr<Label> EntityInformationView::addSection()
{
    auto divider = manager()->createElement<HorizontalRule>(this, Colour::White);
    divider->setPreferredContentWidth(80);
    
    m_dividers.push_back(divider);
    
    auto label = manager()->createElement<Label>( this );
    label->setTextSize(14);
    label->setTextColour(Colour::White);
    label->setPadding(4);
    
    m_sections.push_back( label );
    
    return label;
}
