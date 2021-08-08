#include <ui/level/EntityInformationView.h>
#include <ui/lib/Manager.h>
#include <resource/ResourceManager.h>
#include <game/Level.h>
#include <components/ActorComponent.h>
#include <game/GameParse.h>
#include <utils/Memory.h>

using namespace UI;


EntityInformationContent::EntityInformationContent( Manager *manager, Element *parent, Level* level )
        : Element(manager, parent), m_level(level) {}

EntityInformationSection::EntityInformationSection( Manager *manager, Element *parent, std::string title,
    std::shared_ptr<EntityInformationContent> content )
    : Element(manager, parent), m_content(content)
{
    setLayout<VerticalLayout>( 4, HAlignment::Left );
    
    if (!title.empty())
    {
        m_title = manager->createElement<Label>( this, title );
        m_title->setTextSize(16);
        m_title->setTextColour(Colour::White);
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
    
    
    m_nameLabel = manager->createElement<Label>( this );
    m_nameLabel->setTextSize(18);
    m_nameLabel->setTextColour(Colour::White);
    
    
    auto metaContent = utils::make_shared_with_type<EntityInformationContent, EntityInfoContent::MetaView>( manager, nullptr, level );
    auto metaView = manager->createElement<EntityInformationSection>( this, "",  metaContent );
    m_sections.push_back(metaView);
    
    auto statsContent = utils::make_shared_with_type<EntityInformationContent, EntityInfoContent::StatsView>( manager, nullptr, level );
    auto statsView = manager->createElement<EntityInformationSection>( this, "Stats",  statsContent );
    m_sections.push_back(statsView);
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
    setPreferredContentSize({250, 60});
}

void EntityInfoContent::MetaView::refresh( EntityRef entity )
{

}


EntityInfoContent::StatsView::StatsView( Manager *manager, Element *parent, Level* level )
    : EntityInformationContent(manager, parent, level)
{
    setLayout<FreeLayout>();
    setPreferredContentSize({250, 100});
    TextStyle ts = { Colour::White, ResourceManager::get().getDefaultFont(), 14 };
    
    auto lStrLabel = manager->createElement<Label>( this, ts, "STR" );
    auto lDexLabel = manager->createElement<Label>( this, ts, "DEX" );
    auto lConLabel = manager->createElement<Label>( this, ts, "CON" );
    auto lIntLabel = manager->createElement<Label>( this, ts, "STR" );
    auto lWisLabel = manager->createElement<Label>( this, ts, "DEX" );
    auto lChaLabel = manager->createElement<Label>( this, ts, "CON" );
    
    m_lStrValue = manager->createElement<Label>( this, ts );
    m_lDexValue = manager->createElement<Label>( this, ts );
    m_lConValue = manager->createElement<Label>( this, ts );
    m_lIntValue = manager->createElement<Label>( this, ts );
    m_lWisValue = manager->createElement<Label>( this, ts );
    m_lChaValue = manager->createElement<Label>( this, ts );
    
    
    int firstColX = 0;
    int secondColX = 30;
    int thirdColX = 60;
    int fourthColX = 90;
    
    int firstRowY = 0;
    int rowSpacing = 16;
    
    lStrLabel->setLocalPosition( firstColX, firstRowY );
    lDexLabel->setLocalPosition( firstColX, firstRowY + rowSpacing );
    lConLabel->setLocalPosition( firstColX, firstRowY + rowSpacing * 2);
    
    m_lStrValue->setLocalPosition( secondColX, firstRowY );
    m_lDexValue->setLocalPosition( secondColX, firstRowY + rowSpacing );
    m_lConValue->setLocalPosition( secondColX, firstRowY + rowSpacing * 2);
    
    lIntLabel->setLocalPosition( thirdColX, firstRowY );
    lWisLabel->setLocalPosition( thirdColX, firstRowY + rowSpacing );
    lChaLabel->setLocalPosition( thirdColX, firstRowY + rowSpacing * 2);
    
    m_lIntValue->setLocalPosition( fourthColX, firstRowY );
    m_lWisValue->setLocalPosition( fourthColX, firstRowY + rowSpacing );
    m_lChaValue->setLocalPosition( fourthColX, firstRowY + rowSpacing * 2);
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
}




