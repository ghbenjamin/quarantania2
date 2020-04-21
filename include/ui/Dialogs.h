#pragma once
#include <ui/Element.h>

namespace UI
{

class Dialog : public Element
{
public:
    Dialog( Manager* manager, Element* parent, std::string const& title, int maxWidth );
    ~Dialog() override = default;

protected:
    int m_maxWidth;

    ElementPtr m_contentHolder;
    ElementPtr m_buttonHolder;
};




class MsgBoxDialog : public Dialog
{
public:
    MsgBoxDialog( Manager* manager, Element* parent, std::string const& title, int maxWidth, std::string const& message );
    ~MsgBoxDialog() override = default;
};


}