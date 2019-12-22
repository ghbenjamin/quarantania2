#include <ui/ContextMenu.h>
#include <ui/Manager.h>
#include <utils/Logging.h>

#include <utility>

UI::ContextMenu::ContextMenu(const UI::ContextMenuList &items, ContextMenuCallback callback)
: m_callback(std::move(callback))
{
    holdLayout();
    setLayout<UI::VerticalLayout>(0, UI::HAlignment::Fill);

    for ( std::size_t i = 0; i < items.size(); i++ )
    {
        if ( items[i].empty() )
        {
            manager()->createElement<UI::ContextMenuSpacer>( this );
        }
        else
        {
            auto ptr = manager()->createElement<UI::ContextMenuItem>( this, items[i], i );
            ptr->addEventCallback(UEventType::Click, [this](UEvent& evt) {
                m_callback( evt.targetElement->asType<UI::ContextMenuItem>()->index() );
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

UI::ContextMenuItem::ContextMenuItem(std::string const &label, std::size_t idx)
: m_label(label), m_idx(idx)
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

std::string const &UI::ContextMenuItem::label()
{
    return m_label;
}

std::size_t UI::ContextMenuItem::index()
{
    return m_idx;
}

UI::ContextMenuSpacer::ContextMenuSpacer()
{
    setPreferredContentSize({15, 15});
}
