#pragma once

#include <ui/lib/TextLog.h>


namespace UI
{

class EventLog : public TextLog
{
public:
    EventLog( Manager* manager, Element* parent);
    ~EventLog() override = default;
};

}