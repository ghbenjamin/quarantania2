#include <ui/lib/Tooltips.h>
#include <resource/ResourceManager.h>
#include <ui/lib/Manager.h>

UI::TooltipItem::TooltipItem(UI::Manager *manager, UI::Element *parent, TooltipData const& data)
        : Element(manager, parent)
{
    auto titleFont = ResourceManager::get().getDefaultFont(16);
    auto contentFont = ResourceManager::get().getDefaultFont(14);

    setLayout<VerticalLayout>( 4, HAlignment::Fill );
    setMaximumOuterSize({TOOLTIP_MAX_WIDTH, 1000});
    setPadding(6);

    auto titleNode = manager->createElement<Label>(this, TextStyle{Colour::Black, titleFont });
    titleNode->setMaximumOuterSize({TOOLTIP_MAX_WIDTH, 1000 });
    titleNode->setText( data.title );

    if ( data.subtitle.has_value() )
    {
        auto subtitleNode = manager->createElement<Label>(this, TextStyle{Colour::Black, contentFont });
        subtitleNode->setMaximumOuterSize({TOOLTIP_MAX_WIDTH, 1000 });
        subtitleNode->setText( *data.subtitle );
    }

    if ( data.content.has_value() )
    {
        auto contentNode = manager->createElement<Label>(this, TextStyle{Colour::Black, contentFont });
        contentNode->setMaximumOuterSize({ TOOLTIP_MAX_WIDTH, 1000 });
        contentNode->setText( *data.content );
    }
    
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border" ).getPatch();
    setBackground( patch );
//    setBackground( Colour::Grey.withAlpha(210) );
}

UI::Tooltip::Tooltip(UI::Manager *manager, UI::Element *parent, const std::vector<TooltipData> &data )
        : Element(manager, parent)
{
    sharedInit();

    for ( auto const& td : data )
    {
        manager->createElement<TooltipItem>( this, td );
    }
}

UI::Tooltip::Tooltip(UI::Manager *manager, UI::Element *parent, const UI::TooltipData &data )
        : Element(manager, parent)
{
    sharedInit();

    manager->createElement<TooltipItem>( this, data );
}

void UI::Tooltip::sharedInit()
{
    setLayout<VerticalLayout>( 4, HAlignment::Left );
    setPadding( 4 );
    setMaximumOuterSize({TOOLTIP_MAX_WIDTH, 1000});
    setId( "tooltip" );
}