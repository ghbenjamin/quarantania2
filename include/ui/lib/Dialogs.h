#pragma once
#include <ui/lib/Element.h>

namespace UI
{

class Button;

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



struct MessageBoxButtonInfo
{
    MessageBoxButtonInfo( std::string const &message, std::function<void()> const &callback );
    
    std::string message;
    std::function<void()> callback;
};


class MsgBoxDialog : public Dialog
{
public:
    MsgBoxDialog( Manager* manager, Element* parent, std::string const& title, int maxWidth,
        std::string const& message, std::vector<MessageBoxButtonInfo> const& buttonData );
    ~MsgBoxDialog() override = default;

private:
    std::vector<std::shared_ptr<Button>> m_buttons;
    std::vector<MessageBoxButtonInfo> const& m_buttonData;
};

}