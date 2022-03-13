#pragma once

#include <ui/lib/Element.h>

namespace UI
{

class TextLog;
class EditLine;

class ConsoleDialog : public Element
{
public:
    ConsoleDialog( Manager *manager, Element* parent );
    ~ConsoleDialog() override = default;
    
private:
    void onEnter();

    std::shared_ptr<TextLog> m_textLog;
    std::shared_ptr<EditLine> m_editLine;
};



}
