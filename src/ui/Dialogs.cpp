#include <ui/Dialogs.h>
#include <ui/Manager.h>

UI::Dialog::Dialog(std::string const &title, std::string const &contents, int maxWidth, bool isModal)
: m_title(title), m_contents(contents), m_maxWidth(maxWidth), m_isModal(isModal)
{
    setBorder( 2, Colour::Grey );
    setBackgroundColour( Colour::White );
    setPreferredContentSize({200, 100});
    setLayout<VerticalLayout>( 1, HAlignment::Fill );

//    auto titleElem = manager()->createElement<Element>(this);
//    titleElem->setLayout<HorizontalLayout>( 1, VAlignment::Fill );
//    titleElem->setBackgroundColour( Colour{50, 50, 50} );
//
//    auto titleText = manager()->createElement<TextNode>( titleElem.get(), TextStyle{ Colour::Black} );
//    titleText->setText(title);
//
//    auto contentElem = manager()->createElement<Element>(this);
//    contentElem->setLayout<HorizontalLayout>( 1, VAlignment::Centre );
//
//
//    auto contentText = manager()->createElement<TextNode>( contentElem.get(), TextStyle{ Colour::Black} );
//    contentText->setText(contents);
//
//    auto buttonsElem = manager()->createElement<Element>(this);
//    buttonsElem->setLayout<HorizontalLayout>( 1, VAlignment::Centre );

    doLayout();
}
