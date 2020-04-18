#include <ui/CharStatsView.h>
#include <ui/Manager.h>
#include <game/Level.h>
#include <components/ActorComponent.h>
#include <utils/Assert.h>
#include <resource/ResourceManager.h>

UI::CharStatsView::CharStatsView(UI::Manager *manager, UI::Element *parent)
: Element(manager, parent)
{
    setLayout<FreeLayout>();
    setId( "player-stats-view" );

    TextStyle titleStyle {
        Colour::Black,
        ResourceManager::get().getFont( "inconsolata-18" )
    };

    m_nameNode = manager->createElement<TextNode>( this, titleStyle );
    m_nameNode->setLocalPosition({10, 10});

    m_classNode = manager->createElement<TextNode>( this );
    m_classNode->setLocalPosition({10, 30});
}

void UI::CharStatsView::reimportItems()
{
    auto player = manager()->level()->getPlayer();

    auto actorC = manager()->level()->getComponents<ActorComponent>( player->ref() );
    Assert( !!actorC );

    m_nameNode->setText( player->data().name );
    m_classNode->setText( fmt::format( "{} {}", player->data().race, player->data().clazz ) );
}
