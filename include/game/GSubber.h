#pragma once


struct GEvent;

class GSubber
{
public:
    GSubber() = default;
    virtual ~GSubber() = default;

    virtual void acceptGEvent( GEvent& ) = 0;

};