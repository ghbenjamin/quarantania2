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
    
    m_titleLabel = manager->createElement<Label>(this);
    m_titleLabel->setTextSize(16);
    m_titleLabel->setTextColour(Colour::White);
    m_titleLabel->setPadding(4);
    
    m_divider1 = manager->createElement<HorizontalRule>(this, Colour::White);
    m_divider1->setPreferredContentWidth(80);

    m_section1 = manager->createElement<Label>( this );
    m_section1->setTextSize(14);
    m_section1->setTextColour(Colour::White);
    m_section1->setPadding(4);

    m_divider2 = manager->createElement<HorizontalRule>(this, Colour::White);
    m_divider2->setPreferredContentWidth(80);

    m_section2 = manager->createElement<Label>( this );
    m_section2->setTextSize(14);
    m_section2->setTextColour(Colour::White);
    m_section2->setPadding(4);

    m_section1->setHidden(true);
    m_divider1->setHidden(true);
    m_section2->setHidden(true);
    m_divider2->setHidden(true);

}

void UI::EntityInformationView::refresh( EntityRef entity )
{
    if (entity == EntityNull)
    {
        return;
    }
 
    m_titleLabel->setText(manager()->level()->getDescriptionForEnt( entity ));
    
    if ( manager()->level()->ecs().entityHas<ActorComponent>( entity ))
    {
        m_section1->setHidden(false);
        m_divider1->setHidden(false);
        m_section1->setText("Debug text!");
    }
    else
    {
        m_section1->setHidden(true);
        m_divider1->setHidden(true);
        m_section2->setHidden(true);
        m_divider2->setHidden(true);
    }
}
