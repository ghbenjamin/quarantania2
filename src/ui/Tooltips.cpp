#include <ui/Tooltips.h>
#include <resource/ResourceManager.h>
#include <ui/Manager.h>

UI::TooltipItem::TooltipItem(UI::Manager *manager, UI::Element *parent, TooltipData const& data, bool longContent)
        : Element(manager, parent)
{
    auto titleFont = ResourceManager::get().getDefaultFont(16);
    auto contentFont = ResourceManager::get().getDefaultFont(14);

    setLayout<VerticalLayout>( 4, HAlignment::Fill );
    setMaximumOuterSize({TOOLTIP_MAX_WIDTH, 1000});

    auto titleNode = manager->createElement<Label>(this, TextStyle{Colour::Black, titleFont });
    titleNode->setMaximumOuterSize({TOOLTIP_MAX_WIDTH, 1000 });
    titleNode->setText( data.title );

    auto subtitleNode = manager->createElement<Label>(this, TextStyle{Colour::Black, contentFont });
    subtitleNode->setMaximumOuterSize({TOOLTIP_MAX_WIDTH, 1000 });
    subtitleNode->setText( data.subtitle );

    if ( longContent )
    {
        auto contentNode = manager->createElement<Label>(this, TextStyle{Colour::Black, contentFont });
        contentNode->setMaximumOuterSize({ TOOLTIP_MAX_WIDTH, 1000 });
        contentNode->setText( data.content );
    }

    setBackground( Colour::Grey.withAlpha(210) );
}

UI::Tooltip::Tooltip(UI::Manager *manager, UI::Element *parent, const std::vector<TooltipData> &data, bool longContent)
        : Element(manager, parent)
{
    sharedInit();

    for ( auto const& td : data )
    {
        manager->createElement<TooltipItem>( this, td, longContent );
    }
}

UI::Tooltip::Tooltip(UI::Manager *manager, UI::Element *parent, const UI::TooltipData &data, bool longContent)
        : Element(manager, parent)
{
    sharedInit();

    manager->createElement<TooltipItem>( this, data, longContent );
}

void UI::Tooltip::sharedInit()
{
    setLayout<VerticalLayout>( 4, HAlignment::Left );
    setPadding( 4 );
    setMaximumOuterSize({TOOLTIP_MAX_WIDTH, 1000});
    setId( "tooltip" );
}
