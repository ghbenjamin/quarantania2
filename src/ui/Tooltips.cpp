#include <ui/Tooltips.h>
#include <resource/ResourceManager.h>
#include <ui/Manager.h>

UI::Tooltip::Tooltip(UI::Manager *manager, UI::Element *parent, TooltipData const& data)
        : Element(manager, parent)
{
    auto titleFont = ResourceManager::get().getFont( "inconsolata-16" );
    auto subFont = ResourceManager::get().getFont( "inconsolata-14" );
    auto contentFont = ResourceManager::get().getFont( "inconsolata-14" );

    setLayout<VerticalLayout>( 4, HAlignment::Fill );
    setPadding( 4 );
    setPreferredContentSize({TOOLTIP_WIDTH, 10});
    setId( "tooltip" );

    auto titleNode = manager->createElement<TextNode>(this, TextStyle{ Colour::Black, titleFont });
    titleNode->setMaximumOuterSize({ TOOLTIP_WIDTH, 100 });
    titleNode->setText( data.title );

    auto subtitleNode = manager->createElement<TextNode>(this, TextStyle{ Colour::Black, subFont });
    subtitleNode->setMaximumOuterSize({ TOOLTIP_WIDTH, 100 });
    subtitleNode->setText( data.subtitle );

//    auto contentNode = manager->createElement<TextNode>(this, TextStyle{ Colour::Black, contentFont });
//    contentNode->setMaximumOuterSize({ TOOLTIP_WIDTH, 100 });
//    contentNode->setText( data.content );

    setBackgroundColour( Colour::Grey.withAlpha(180) );
}
