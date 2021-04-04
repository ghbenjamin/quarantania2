#include <ui/lib/Tooltips.h>
#include <resource/ResourceManager.h>
#include <ui/lib/Manager.h>

UI::TooltipItem::TooltipItem(UI::Manager *manager, UI::Element *parent, TooltipData const& data)
        : Element(manager, parent)
{
    auto titleFont = ResourceManager::get().getDefaultFont();
    auto contentFont = ResourceManager::get().getDefaultFont();

    setLayout<VerticalLayout>( 4, HAlignment::Fill );
    setMaximumOuterSize({TOOLTIP_MAX_WIDTH, 1000});
    setPadding(6);

    auto titleNode = manager->createElement<Label>(this, TextStyle{Colour::White, titleFont, 16 });
    titleNode->setMaximumOuterSize({TOOLTIP_MAX_WIDTH, 1000 });
    titleNode->setText( data.title );

    if ( data.subtitle.has_value() && !(*(data.subtitle)).empty() )
    {
        auto subtitleNode = manager->createElement<Label>(this, TextStyle{Colour::White, contentFont, 14 });
        subtitleNode->setMaximumOuterSize({TOOLTIP_MAX_WIDTH, 1000 });
        subtitleNode->setText( *(data.subtitle) );
    }

    if ( data.content.has_value() && !(*(data.content)).empty() )
    {
        auto contentNode = manager->createElement<Label>(this, TextStyle{Colour::White, contentFont, 14 });
        contentNode->setMaximumOuterSize({ TOOLTIP_MAX_WIDTH, 1000 });
        contentNode->setText( *(data.content) );
    }
    
    if ( outerBounds().w() < TOOLTIP_MIN_WIDTH )
    {
        setPreferredContentWidth( TOOLTIP_MIN_WIDTH );
    }
    
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border" ).getPatch();
    setBackground( patch );
    setBorderWidth( patch.getBorderWidth() );
}

UI::Tooltip::Tooltip(UI::Manager *manager, UI::Element *parent, const std::vector<TooltipData> &data )
        : Element(manager, parent)
{
    sharedInit();

    for ( auto const& td : data )
    {
        manager->createElement<TooltipItem>( this, td );
    }
    
    // Delete tooltips if someone manages to hover one
    addEventCallback( UEventType::MouseIn, [this](UEvent& evt) {
        this->manager()->deleteElement( this->shared_from_this() );
    });
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