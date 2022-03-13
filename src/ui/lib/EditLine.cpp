#include <ui/lib/EditLine.h>
#include <ui/lib/Manager.h>

using namespace UI;

EditLine::EditLine( Manager *manager, Element *parent )
: Element(manager, parent)
{
    setLayout<FreeLayout>();
    addEventCallback( UEventType::TextInput, [this](UEvent& evt){ this->onTextInput(evt); });
    addEventCallback( UEventType::KeyPress, [this](UEvent& evt){ this->onKeyPress(evt); });
    
    m_label = manager->createElement<Label>( this );
    m_label->setLocalPosition(0, 0);
    
    m_caret = manager->createElement( this );
    m_caret->setLocalPosition( 0, 0 );
    m_caret->setPreferredContentSize({2, 14});
    
    m_caret->setBackground( Colour::Black );
}

void EditLine::onTextInput( UEvent &evt )
{
    // TODO: Hacky nonsense
    if ( strcmp( evt.textInputEvent.text, "`" ) == 0 )
    {
        return;
    }

    m_buffer += evt.textInputEvent.text;
    rerender();
}

void EditLine::onKeyPress( UEvent &evt )
{
    switch ( evt.keyEvent.keyCode )
    {
        case SDLK_BACKSPACE:
        {
            if ( !m_buffer.empty() )
            {
                m_buffer.pop_back();
                rerender();
            }
            
            break;
        }
    
        case SDLK_SPACE:
        {
            m_buffer += " ";
            rerender();
        }
    }
}

void EditLine::rerender()
{
    m_label->setText( m_buffer );
    
    auto labelBounds = m_label->outerBounds();
    m_caret->setLocalPosition( labelBounds.w() + 4, 0 );
}

void EditLine::reset()
{
    m_buffer = " ";
    rerender();
}

std::string const &EditLine::getText() const
{
    return m_buffer;
}
