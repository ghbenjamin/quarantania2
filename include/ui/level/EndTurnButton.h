#pragma once

#include <ui/lib/Element.h>
#include <ui/lib/Controls.h>

namespace UI
{

class EndTurnButton : public Button
{
public:
    EndTurnButton(Manager* manager, Element* parent);
    ~EndTurnButton() override = default;

private:
    void onClick();
};

}