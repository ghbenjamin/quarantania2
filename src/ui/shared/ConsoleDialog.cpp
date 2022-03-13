#include <ui/shared/ConsoleDialog.h>
#include <ui/lib/Manager.h>
#include <ui/lib/TextLog.h>
#include <ui/lib/EditLine.h>

using namespace UI;

ConsoleDialog::ConsoleDialog( Manager *manager, Element* parent )
: Element(manager, parent)
{
    setLayout<VerticalLayout>( 4, HAlignment::Fill );
    
    setBackground( Colour::Grey.withAlphaF( 0.4 ) );
    setDecorative();
    setPadding( 6 );
    
    m_textLog = manager->createElement<TextLog>( this );
    m_editLine = manager->createElement<EditLine>( this );
    
    m_editLine->addEventCallback( UEventType::KeyPress, [this]( UEvent& evt ){
        if ( evt.keyEvent.keyCode == SDLK_RETURN )
        {
            onEnter();
        }
    });
    
    m_editLine->setPreferredContentSize({1, 16});
    m_textLog->setPreferredContentSize({1, 80});
    
    m_textLog->addLine( "Hello, console!" );
    
    manager->setFocusedElement( m_editLine );
}

void ConsoleDialog::onEnter()
{
    auto current = m_editLine->getText();
    m_editLine->reset();
    
    if ( !current.empty() )
    {
        m_textLog->addLine( current );
    }
}
