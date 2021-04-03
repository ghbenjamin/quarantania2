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
    setPreferredContentSize({100, 50});
    
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border" );
    NinePatch np = { patch.texture(), patch.offsets() };
    setBackground( np );
    setBorderWidth( patch.borderWidth() );
    
    setLayout<VerticalLayout>( 2, HAlignment::Centre );
    
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
    
    if ( manager()->level()->ecs().entityHas<ActorComponent>( entity ))
    {
    
    }
    else
    {
    
    }
}
