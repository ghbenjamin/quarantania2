#include <ui/Dialogs.h>
#include <ui/Manager.h>

UI::Dialog::Dialog(Manager* manager, Element* parent, std::string const &title, std::string const &contents, int maxWidth, bool isModal)
: Element(manager, parent), m_title(title), m_contents(contents), m_maxWidth(maxWidth), m_isModal(isModal)
{
    setBorder( 2, Colour::Grey );
    setBackgroundColour( Colour::White );
    setPreferredContentSize({200, 100});
    setLayout<VerticalLayout>( 1, HAlignment::Fill );


    doLayout();
}
