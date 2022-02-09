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
    
    auto featsContent = utils::make_shared_with_type<EntityInformationContent,
            EntityInfoContent::FeatsView>( manager, nullptr, level );
    auto featsView = manager->createElement<EntityInformationSection>( this, "Feats",  featsContent );
    m_sections.push_back(featsView);
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
             m_sections[(int)EntityInformationSectionType::Feats]->show();
        }
        else
        {
            m_sections[(int)EntityInformationSectionType::Stats]->hide();
            m_sections[(int)EntityInformationSectionType::Feats]->hide();
        }
    }
    else
    {
        m_sections[(int)EntityInformationSectionType::Stats]->hide();
        m_sections[(int)EntityInformationSectionType::Feats]->hide();
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
    
    m_lStrValue = manager->createElement<TaggedLabel>( this, ts );
    m_lDexValue = manager->createElement<TaggedLabel>( this, ts );
    m_lConValue = manager->createElement<TaggedLabel>( this, ts );
    m_lIntValue = manager->createElement<TaggedLabel>( this, ts );
    m_lWisValue = manager->createElement<TaggedLabel>( this, ts );
    m_lChaValue = manager->createElement<TaggedLabel>( this, ts );


    auto lFortLabel = manager->createElement<Label>( this, ts, "FORT" );
    auto lRefLabel = manager->createElement<Label>( this, ts, "REF" );
    auto lWillLabel = manager->createElement<Label>( this, ts, "WILL" );

    m_lFortValue = manager->createElement<TaggedLabel>( this, ts );
    m_lRefValue = manager->createElement<TaggedLabel>( this, ts );
    m_lWillValue = manager->createElement<TaggedLabel>( this, ts );


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
    
    m_lStrValue->refresh( actor.getAbilityScoreValue(AbilityScoreType::STR),
        actor.getStaticModifier(ActorStaticModType::AttrStr) );
    m_lDexValue->refresh( actor.getAbilityScoreValue(AbilityScoreType::DEX),
        actor.getStaticModifier(ActorStaticModType::AttrDex) );
    m_lConValue->refresh( actor.getAbilityScoreValue(AbilityScoreType::CON),
        actor.getStaticModifier(ActorStaticModType::AttrCon) );
    m_lIntValue->refresh( actor.getAbilityScoreValue(AbilityScoreType::INT),
        actor.getStaticModifier(ActorStaticModType::AttrInt) );
    m_lWisValue->refresh( actor.getAbilityScoreValue(AbilityScoreType::WIS),
        actor.getStaticModifier(ActorStaticModType::AttrWis) );
    m_lChaValue->refresh( actor.getAbilityScoreValue(AbilityScoreType::CHA),
        actor.getStaticModifier(ActorStaticModType::AttrCha) );

    m_lFortValue->refresh( actor.getFortSave(), actor.getStaticModifier(ActorStaticModType::SaveFort) );
    m_lRefValue->refresh( actor.getRefSave(), actor.getStaticModifier(ActorStaticModType::SaveRef));
    m_lWillValue->refresh( actor.getWillSave(), actor.getStaticModifier(ActorStaticModType::SaveWill) );
}


EntityInfoContent::StatusView::StatusView(Manager *manager, Element *parent, Level *level)
        : EntityInformationContent(manager, parent, level)
{

}

void EntityInfoContent::StatusView::refresh(EntityRef entity)
{
    if ( entity == EntityNull )
    {
        return;
    }
    
    for ( auto const& s : m_statuses )
    {
        manager()->deleteElement( s.get() );
    }
    
    if (! m_level->ecs().entityHas<ActorComponent>(entity))
    {
        return;
    }
    
    auto& actor = m_level->ecs().getComponents<ActorComponent>( entity )->actor;
    
    for ( auto const mod : actor.getAllModifiers() )
    {
        if ( mod.getId().starts_with("status"))
        {
            auto label = manager()->createElement<Label>( this, mod.getName() );
            label->setTextColour(Colour::White);
            label->setTooltipSpawner( [mod, this] () {
                auto data = ResourceDatabase::get().modifierFromId(mod.getId());
                return manager()->createElement<SimpleTooltip>(nullptr, data.name, data.effect);
            });
            m_statuses.push_back(label);
        }
    }
}

EntityInfoContent::TaggedLabel::TaggedLabel( Manager *manager, Element *parent, TextStyle ts )
    : Label(manager, parent, ts)
{

}

void EntityInfoContent::TaggedLabel::refresh(int value, int modifier)
{
    setText( std::to_string(value) );
    
    if ( modifier > 0 )
    {
        setTextColour(Colour::Lime);
    }
    else if ( modifier < 0 )
    {
        setTextColour(Colour::Red);
    }
    else
    {
        setTextColour(Colour::White);
    }
}

EntityInfoContent::FeatsView::FeatsView( Manager *manager, Element *parent, Level *level ) : EntityInformationContent(
        manager, parent, level)
{
    setLayout<VerticalLayout>( 4, HAlignment::Left );
}

void EntityInfoContent::FeatsView::refresh( EntityRef entity )
{
    if ( entity == EntityNull )
    {
        return;
    }

    for ( auto const& f : m_feats )
    {
        manager()->deleteElement( f.get() );
    }
    
    m_feats.clear();
    
    auto& actor = m_level->ecs().getComponents<ActorComponent>( entity )->actor;
    
    for ( auto const mod : actor.getAllModifiers() )
    {
        if ( mod.getId().starts_with("feat"))
        {
            auto label = manager()->createElement<Label>( this, mod.getName() );
            label->setTextColour(Colour::White);
            label->setTooltipSpawner( [mod, this] () {
                auto data = ResourceDatabase::get().modifierFromId(mod.getId());
                return manager()->createElement<SimpleTooltip>(nullptr, data.name, data.effect);
            });
            m_feats.push_back(label);
        }
    }
}
