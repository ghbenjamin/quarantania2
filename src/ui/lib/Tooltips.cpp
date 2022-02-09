#include <ui/lib/Tooltips.h>
#include <resource/ResourceManager.h>
#include <ui/lib/Manager.h>

using namespace UI; 

SimpleTooltipItem::SimpleTooltipItem( Manager *manager, Element *parent, std::string const& title, std::string const& content)
        : Element(manager, parent)
{
    auto titleFont = ResourceManager::get().getDefaultFont();
    auto contentFont = ResourceManager::get().getDefaultFont();

    setLayout<VerticalLayout>( 4, HAlignment::Fill );
    setMaximumOuterSize({TOOLTIP_MAX_WIDTH, 1000});
    setPadding(6);

    auto titleNode = manager->createElement<Label>(this, TextStyle{Colour::White, titleFont, 16 });
    titleNode->setMaximumOuterSize({TOOLTIP_MAX_WIDTH, 1000 });
    titleNode->setText( title );

    if ( !content.empty() )
    {
        auto contentNode = manager->createElement<Label>(this, TextStyle{Colour::White, contentFont, 14 });
        contentNode->setMaximumOuterSize({ TOOLTIP_MAX_WIDTH, 1000 });
        contentNode->setText( content );
    }
    
    if ( outerBounds().w() < TOOLTIP_MIN_WIDTH )
    {
        setPreferredContentWidth( TOOLTIP_MIN_WIDTH );
    }
    
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border" ).getPatch();
    setBackground( patch );
    setBorderWidth( patch.getBorderWidth() );
}

Tooltip::Tooltip(Manager *manager, Element *parent )
        : Element(manager, parent)
{
    setLayout<VerticalLayout>( 4, HAlignment::Left );
    setPadding( 4 );
    setMaximumOuterSize({TOOLTIP_MAX_WIDTH, 1000});
    setId( "tooltip" );
    
    // Delete tooltips if someone manages to hover one
    addEventCallback( UEventType::MouseIn, [this](UEvent& evt) {
        this->manager()->deleteElement( this );
    });
}


SimpleTooltip::SimpleTooltip( Manager *manager, Element *parent, std::string const& title, std::string const& content)
: Tooltip(manager, parent)
{
    manager->createElement<SimpleTooltipItem>(this, title, content);
}
