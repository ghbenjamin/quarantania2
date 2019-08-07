#include <ui/ContextMenu.h>
#include <ui/UiManager.h>

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
            manager()->createElement<UI::Internal::ContextMenuItem>( this, item );
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
}

UI::Internal::ContextMenuSpacer::ContextMenuSpacer()
{
    setPreferredContentSize({15, 15});
}
