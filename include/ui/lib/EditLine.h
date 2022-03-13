#pragma once

#include <ui/lib/Element.h>

namespace UI
{

class Label;

class EditLine : public Element
{
public:
    EditLine( Manager *manager, Element *parent );
    
    void reset();
    std::string const& getText() const;
    
private:

    void onTextInput( UEvent& evt );
    void onKeyPress( UEvent& evt );
    void rerender();

    std::shared_ptr<Label> m_label;
    std::shared_ptr<Element> m_caret;
    
    std::string m_buffer;
};


}