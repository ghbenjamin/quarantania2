#include <ui/lib/ContextMenu.h>
#include <ui/lib/Manager.h>
#include <utils/Logging.h>

#include <utility>

UI::ContextMenu::ContextMenu(Manager* manager, Element* parent, const UI::ContextMenuList &items, ContextMenuCallback callback)
: Element(manager, parent), m_callback(std::move(callback))
{
    setLayout<UI::VerticalLayout>(0, UI::HAlignment::Fill);

    for ( std::size_t i = 0; i < items.size(); i++ )
    {
        if ( items[i].empty() )
        {
            manager->createElement<UI::ContextMenuSpacer>( this );
        }
        else
        {
            auto ptr = manager->createElement<UI::ContextMenuItem>( this, items[i], i );
            ptr->addEventCallback(UEventType::Click, [this](UEvent& evt) {
                m_callback( evt.targetElement->asType<UI::ContextMenuItem>()->index() );
                this->manager()->deleteElement( shared_from_this() );
            });
        }
    }

    setBorder( Colour::Beige, 2, Colour::Black );
    setPadding( 6 );
    setId( "context-menu" );
}

UI::ContextMenuItem::ContextMenuItem(Manager* manager, Element* parent, std::string const &label, std::size_t idx)
    : Element(manager, parent), m_label(label), m_idx(idx)
{
    auto tnode = manager->createElement<UI::Label>(this );
    tnode->setText( label );
    setPadding(0);

    addEventCallback( UEventType::MouseIn, [this](UEvent& evt) {
        setBackground( Colour::Green );
    });

    addEventCallback( UEventType::MouseOut, [this](UEvent& evt) {
        removeBackround();
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

UI::ContextMenuSpacer::ContextMenuSpacer(Manager* manager, Element* parent)
    : Element(manager, parent)
{
    setPreferredContentSize({15, 15});
}
