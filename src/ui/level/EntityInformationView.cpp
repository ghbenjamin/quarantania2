#include <ui/level/EntityInformationView.h>
#include <ui/lib/Manager.h>
#include <resource/ResourceManager.h>
#include <game/Level.h>
#include <components/ActorComponent.h>
#include <game/GameParse.h>

using namespace UI;


EntityInformationContent::EntityInformationContent( Manager *manager, Element *parent )
        : Element(manager, parent) {}

EntityInformationSection::EntityInformationSection( Manager *manager, Element *parent, std::string title,
    std::shared_ptr<EntityInformationContent> content )
    : Element(manager, parent), m_content(content)
{
    setLayout<VerticalLayout>( 2, HAlignment::Centre );
    manager->createElement<Label>( this, title );
}

void EntityInformationSection::refresh( EntityRef entity )
{
    m_content->refresh(entity);
}


EntityInformationView::EntityInformationView( Manager *manager, Element *parent, Level* level )
    : Element(manager, parent), m_level(level)
{
    setId("entity-information-view");
    
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border" ).getPatch();
    setBackground( patch );
    setBorderWidth( patch.getBorderWidth() );
    setLayout<VerticalLayout>( 2, HAlignment::Centre );
    
    setPreferredContentSize({200, 10});
    
    m_nameLabel = manager->createElement<Label>( this );
}

void EntityInformationView::refresh( EntityRef entity )
{
    

    for ( auto const& sec : m_sections )
    {
        sec->refresh(entity);
    }
}

