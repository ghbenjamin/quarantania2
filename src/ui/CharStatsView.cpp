#include <ui/CharStatsView.h>
#include <ui/Manager.h>
#include <game/Level.h>
#include <components/ActorComponent.h>
#include <utils/Assert.h>
#include <resource/ResourceManager.h>

UI::CharStatsView::CharStatsView(UI::Manager *manager, UI::Element *parent)
: Element(manager, parent)
{
    static const Vector2i TITLE_OFFSET = {10, 10};
    static const Vector2i CLASS_RACE_OFFSET = {10, 30};

    static const Vector2i ABILITY_SCORE_HEADING_OFFSET = {10, 50};
    static const Vector2i ABILITY_SCORE_VALUE_OFFSET = {40, 50};
    static const Vector2i ABILITY_SCORE_MODIFIER_OFFSET = {70, 50};
    static const int ABILITY_SCORE_HEADING_V_SEP = 20;

    setLayout<FreeLayout>();
    setId( "player-stats-view" );

    TextStyle titleStyle {
        Colour::Black,
        ResourceManager::get().getFont( "inconsolata-18" )
    };

    m_nameNode = manager->createElement<TextNode>( this, titleStyle );
    m_nameNode->setLocalPosition(TITLE_OFFSET);

    m_classNode = manager->createElement<TextNode>( this );
    m_classNode->setLocalPosition(CLASS_RACE_OFFSET);


    auto abilityScores = { "STR", "DEX", "CON", "INT", "WIS", "CHA" };
    int i = 0;
    for ( auto const& as : abilityScores )
    {
        m_abilityScoreHeaders.push_back( manager->createElement<TextNode>( this ) );
        m_abilityScoreHeaders.back()->setText(as);
        m_abilityScoreHeaders.back()->setLocalPosition({
                ABILITY_SCORE_HEADING_OFFSET.x(),
                ABILITY_SCORE_HEADING_OFFSET.y() + i * ABILITY_SCORE_HEADING_V_SEP
        });

        m_abilityScoreValues.push_back( manager->createElement<TextNode>( this ) );
        m_abilityScoreValues.back()->setLocalPosition({
                                     ABILITY_SCORE_VALUE_OFFSET.x(),
                                     ABILITY_SCORE_VALUE_OFFSET.y() + i * ABILITY_SCORE_HEADING_V_SEP
                             });

        m_abilityScoreModifiers.push_back( manager->createElement<TextNode>( this ) );
        m_abilityScoreModifiers.back()->setLocalPosition({
                                    ABILITY_SCORE_MODIFIER_OFFSET.x(),
                                    ABILITY_SCORE_MODIFIER_OFFSET.y() + i * ABILITY_SCORE_HEADING_V_SEP
                            });

        i++;
    }
}

void UI::CharStatsView::reimportItems()
{
    auto player = manager()->level()->getPlayer();

    auto actorC = manager()->level()->getComponents<ActorComponent>( player->ref() );
    Assert( !!actorC );

    m_nameNode->setText( player->data().name );
    m_classNode->setText( fmt::format( "{} {}", player->data().race, player->data().clazz ) );


    for ( int i = 0; i < 6; i++ )
    {
        m_abilityScoreValues[i]->setText( fmt::format( "{}", actorC->getIthAbilityScore(i) ) );
        m_abilityScoreModifiers[i]->setText( fmt::format( "{:+d}", actorC->getIthAbilityScoreMod(i) ) );
    }

}

