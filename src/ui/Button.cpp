#include <ui/Button.h>
#include <ui/Manager.h>

UI::Button::Button(Manager *manager, Element *parent, const std::string &text, std::function<void()> const& callback)
    : Element(manager, parent), m_callback(callback)
{
    setPadding( 4 );
    setBackgroundColour({200, 200, 200, 255});

    auto textNode = manager->createElement<UI::TextNode>( this );
    textNode->setText( text );

    addEventCallback(UEventType::Click, [this] (UEvent& evt) {
        this->m_callback();
    });
}
