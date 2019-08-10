#include <ui/ContextMenu.h>
#include <ui/UiManager.h>
#include <utils/Logging.h>

UI::ContextMenu::ContextMenu(const UI::ContextMenuList &items)
{
    holdLayout();

    for ( auto const& item : items )
    {
        if ( item.empty() )
        {
            manager()->createElement<UI::Internal::ContextMenuSpacer>( this );
        }
        else
        {
            auto ptr = manager()->createElement<UI::Internal::ContextMenuItem>( this, item );
            ptr->addEventCallback(UEventType::Click, [this](UEvent& evt) {
                Logging::log( evt.mouseButtonEvent.pos );
                Logging::log( evt.mouseButtonEvent.button );
            });
        }
    }

    setBackgroundColour( Colour::Beige );
    setBorder( 2, Colour::Black );
    setPadding(6);
    releaseLayout();
}

UI::Internal::ContextMenuItem::ContextMenuItem(std::string const &label)
{
    auto tnode = manager()->createElement<UI::TextNode>( this );
    tnode->setText( label );

    addEventCallback( UEventType::MouseIn, [this](UEvent& evt) {
        setBackgroundColour( Colour::Green );
    });

    addEventCallback( UEventType::MouseOut, [this](UEvent& evt) {
        removeBackgroundColour();
    });
}

UI::Internal::ContextMenuSpacer::ContextMenuSpacer()
{
    setPreferredContentSize({15, 15});
}
