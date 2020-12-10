#include <ui/Dialogs.h>
#include <ui/Manager.h>
#include <resource/ResourceManager.h>
#include <ui/Controls.h>

UI::Dialog::Dialog(Manager* manager, Element* parent, std::string const &title, int maxWidth )
: Element(manager, parent), m_maxWidth(maxWidth)
{
    setBorder( 2, Colour::Grey );
    setBackgroundColour( Colour::White );
    setMaximumOuterSize({maxWidth, 1000});
    setLayout<VerticalLayout>( 1, HAlignment::Left );

    auto titleFont = ResourceManager::get().getDefaultFont(14);

    auto titleBar = manager->createElement<Element>( this );
    titleBar->setLayout<HorizontalLayout>( 0, VAlignment::Centre );
    titleBar->setPadding(4);

    auto titleText = manager->createElement<Label>(titleBar.get(), TextStyle{Colour::Black, titleFont } );
    titleText->setText( title );

    m_contentHolder = manager->createElement<Element>( this );
    m_contentHolder->setLayout<VerticalLayout>( 0, HAlignment::Left );
    m_contentHolder->setPadding(4);

    m_buttonHolder = manager->createElement<Element>( this );
    m_buttonHolder->setLayout<HorizontalLayout>( 4, VAlignment::Centre );
    m_buttonHolder->setPadding(4);
}

UI::MsgBoxDialog::MsgBoxDialog(UI::Manager *manager, UI::Element *parent, std::string const &title, int maxWidth,
                               std::string const &message)
       : Dialog(manager, parent, title, maxWidth)
{
    auto contentFont = ResourceManager::get().getDefaultFont(14);

    auto textContent = manager->createElement<Label>(m_contentHolder.get(), TextStyle{Colour::Black, contentFont });
    textContent->setText( message );

    manager->createElement<Button>( m_buttonHolder.get(), "Yes", [](){
        Logging::log( "YES" );
    });

    manager->createElement<Button>( m_buttonHolder.get(), "No", [](){
        Logging::log( "NO" );
    });

    doLayout();
}
