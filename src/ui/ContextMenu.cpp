#include <ui/ContextMenu.h>
#include <ui/Manager.h>
#include <utils/Logging.h>

#include <utility>

UI::ContextMenu::ContextMenu(const UI::ContextMenuList &items, ContextMenuCallback callback)
: m_callback(std::move(callback))
{
    holdLayout();
    setLayout<UI::VerticalLayout>(0, UI::HAlignment::Fill);

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
                m_callback( evt.targetElement->asType<UI::Internal::ContextMenuItem>()->label() );
                manager()->deleteElement( shared_from_this() );
            });
        }
    }

    setBackgroundColour( Colour::Beige );
    setBorder( 2, Colour::Black );
    setPadding(6);
    setId("context-menu");
    releaseLayout();
}

UI::Internal::ContextMenuItem::ContextMenuItem(std::string const &label)
: m_label(label)
{
    auto tnode = manager()->createElement<UI::TextNode>( this );
    tnode->setText( label );
    setPadding(0);

    addEventCallback( UEventType::MouseIn, [this](UEvent& evt) {
        setBackgroundColour( Colour::Green );
    });

    addEventCallback( UEventType::MouseOut, [this](UEvent& evt) {
        removeBackgroundColour();
    });
}

std::string const &UI::Internal::ContextMenuItem::label()
{
    return m_label;
}

UI::Internal::ContextMenuSpacer::ContextMenuSpacer()
{
    setPreferredContentSize({15, 15});
}
