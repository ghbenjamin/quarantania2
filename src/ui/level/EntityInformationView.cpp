#include <ui/level/EntityInformationView.h>
#include <ui/lib/Manager.h>
#include <resource/ResourceManager.h>
#include <game/Level.h>
#include <components/ActorComponent.h>
#include <utils/Memory.h>

using namespace UI;


EntityInformationContent::EntityInformationContent( Manager *manager, Element *parent, Level* level )
        : Element(manager, parent), m_level(level) {}

EntityInformationSection::EntityInformationSection( Manager *manager, Element *parent, std::string title,
    std::shared_ptr<EntityInformationContent> content )
    : Element(manager, parent), m_content(content)
{
    setLayout<VerticalLayout>( 4, HAlignment::Left );
    setPreferredContentWidth(250);

    if (!title.empty())
    {
        auto container = manager->createElement( this );
        container->setLayout<HorizontalLayout>(8, VAlignment::Centre );
        container->setPreferredContentWidth(250);

        m_title = manager->createElement<Label>( container.get(), title );
        m_title->setTextSize(16);
        m_title->setTextColour(Colour::White);

        auto hrule = manager->createElement<HorizontalRule>( container.get(), Colour::Grey );
        hrule->setPreferredContentWidth( 250 - m_title->outerBounds().w() - 8 );
    }
    
    m_content = content;
    addChild( content );
    content->setParent(this);
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

    setLayout<VerticalLayout>( 6, HAlignment::Centre );
    setPadding(4);

    m_nameLabel = manager->createElement<Label>( this );
    m_nameLabel->setTextSize(18);
    m_nameLabel->setTextColour(Colour::White);
    
    
    auto metaContent = utils::make_shared_with_type<EntityInformationContent,
        EntityInfoContent::MetaView>( manager, nullptr, level );
    auto metaView = manager->createElement<EntityInformationSection>( this, "",  metaContent );
    m_sections.push_back(metaView);
    
    auto statsContent = utils::make_shared_with_type<EntityInformationContent,
        EntityInfoContent::StatsView>( manager, nullptr, level );
    auto statsView = manager->createElement<EntityInformationSection>( this, "Attributes",  statsContent );
    m_sections.push_back(statsView);

    auto statusContent = utils::make_shared_with_type<EntityInformationContent,
            EntityInfoContent::StatusView>( manager, nullptr, level );
    auto statusView = manager->createElement<EntityInformationSection>( this, "Status",  statusContent );
    m_sections.push_back(statusView);
}

void EntityInformationView::refresh( EntityRef entity )
{
    if ( entity == EntityNull )
    {
        return;
    }

    m_nameLabel->setText( m_level->getDescriptionForEnt( entity ) );
    
    
    auto actorC = m_level->ecs().tryGetComponent<ActorComponent>( entity );
    if ( actorC )
    {
        if ( actorC->actorType == ActorType::PC )
        {
             m_sections[(int)EntityInformationSectionType::Stats]->show();
        }
        else
        {
            m_sections[(int)EntityInformationSectionType::Stats]->hide();
        }
    }
    else
    {
        m_sections[(int)EntityInformationSectionType::Stats]->hide();
    }
    

    for ( auto const& sec : m_sections )
    {
        if ( !sec->isHidden() )
        {
            sec->refresh(entity);
        }
    }
}



EntityInfoContent::MetaView::MetaView( Manager *manager, Element *parent, Level* level )
    : EntityInformationContent(manager, parent, level)
{
    setLayout<VerticalLayout>( 4, HAlignment::Centre );
    TextStyle ts = { Colour::White, ResourceManager::get().getDefaultFont(), 14 };
    setPreferredContentWidth(250);

    m_lEntityType = manager->createElement<Label>( this, ts );
}

void EntityInfoContent::MetaView::refresh( EntityRef entity )
{
    if ( entity == EntityNull )
    {
        return;
    }

    auto actorC = m_level->ecs().tryGetComponent<ActorComponent>(entity);

    if ( actorC )
    {
        m_lEntityType->setText( fmt::format( "Creature - {}", actorC->actor.getCreatureType() ) );
    }
    else
    {
        m_lEntityType->setText( "Object" );
    }
}


EntityInfoContent::StatsView::StatsView( Manager *manager, Element *parent, Level* level )
    : EntityInformationContent(manager, parent, level)
{
    setLayout<FreeLayout>();
    setPreferredContentSize({250, 60});
    TextStyle ts = { Colour::White, ResourceManager::get().getDefaultFont(), 16 };

    // Label construction

    auto lStrLabel = manager->createElement<Label>( this, ts, "STR" );
    auto lDexLabel = manager->createElement<Label>( this, ts, "DEX" );
    auto lConLabel = manager->createElement<Label>( this, ts, "CON" );
    auto lIntLabel = manager->createElement<Label>( this, ts, "INT" );
    auto lWisLabel = manager->createElement<Label>( this, ts, "WIS" );
    auto lChaLabel = manager->createElement<Label>( this, ts, "CHA" );
    
    m_lStrValue = manager->createElement<Label>( this, ts );
    m_lDexValue = manager->createElement<Label>( this, ts );
    m_lConValue = manager->createElement<Label>( this, ts );
    m_lIntValue = manager->createElement<Label>( this, ts );
    m_lWisValue = manager->createElement<Label>( this, ts );
    m_lChaValue = manager->createElement<Label>( this, ts );


    auto lFortLabel = manager->createElement<Label>( this, ts, "FORT" );
    auto lRefLabel = manager->createElement<Label>( this, ts, "REF" );
    auto lWillLabel = manager->createElement<Label>( this, ts, "WILL" );

    m_lFortValue = manager->createElement<Label>( this, ts );
    m_lRefValue = manager->createElement<Label>( this, ts );
    m_lWillValue = manager->createElement<Label>( this, ts );


    // Layout

    int firstRowY = 0;
    int rowSpacing = 18;
    int colX = 0;

    lStrLabel->setLocalPosition( colX, firstRowY );
    lDexLabel->setLocalPosition( colX, firstRowY + rowSpacing );
    lConLabel->setLocalPosition( colX, firstRowY + rowSpacing * 2);
    colX += 34;

    m_lStrValue->setLocalPosition( colX, firstRowY );
    m_lDexValue->setLocalPosition( colX, firstRowY + rowSpacing );
    m_lConValue->setLocalPosition( colX, firstRowY + rowSpacing * 2);
    colX += 60;


    lIntLabel->setLocalPosition( colX, firstRowY );
    lWisLabel->setLocalPosition( colX, firstRowY + rowSpacing );
    lChaLabel->setLocalPosition( colX, firstRowY + rowSpacing * 2);
    colX += 34;

    m_lIntValue->setLocalPosition( colX, firstRowY );
    m_lWisValue->setLocalPosition( colX, firstRowY + rowSpacing );
    m_lChaValue->setLocalPosition( colX, firstRowY + rowSpacing * 2);
    colX += 60;


    lFortLabel->setLocalPosition( colX, firstRowY );
    lRefLabel->setLocalPosition( colX, firstRowY + rowSpacing );
    lWillLabel->setLocalPosition( colX, firstRowY + rowSpacing * 2);
    colX += 40;

    m_lFortValue->setLocalPosition( colX, firstRowY );
    m_lRefValue->setLocalPosition( colX, firstRowY + rowSpacing );
    m_lWillValue->setLocalPosition( colX, firstRowY + rowSpacing * 2);

}

void EntityInfoContent::StatsView::refresh( EntityRef entity )
{
    auto& actor = m_level->ecs().getComponents<ActorComponent>( entity )->actor;
    
    m_lStrValue->setText( std::to_string( actor.getAbilityScoreValue(AbilityScoreType::STR)) );
    m_lDexValue->setText( std::to_string( actor.getAbilityScoreValue(AbilityScoreType::DEX)) );
    m_lConValue->setText( std::to_string( actor.getAbilityScoreValue(AbilityScoreType::CON)) );
    m_lIntValue->setText( std::to_string( actor.getAbilityScoreValue(AbilityScoreType::INT)) );
    m_lWisValue->setText( std::to_string( actor.getAbilityScoreValue(AbilityScoreType::WIS)) );
    m_lChaValue->setText( std::to_string( actor.getAbilityScoreValue(AbilityScoreType::CHA)) );

    m_lFortValue->setText( std::to_string( actor.getFortSave()) );
    m_lRefValue->setText( std::to_string( actor.getRefSave()) );
    m_lWillValue->setText( std::to_string( actor.getWillSave()) );
}


EntityInfoContent::StatusView::StatusView(Manager *manager, Element *parent, Level *level)
        : EntityInformationContent(manager, parent, level)
{

}

void EntityInfoContent::StatusView::refresh(EntityRef entity)
{

}
