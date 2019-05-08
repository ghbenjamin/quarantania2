#pragma once

#include "SDL.h"

class Timer
{
public:
    Timer();
    ~Timer();

    void start();
    void stop();

    Uint32 elapsed();

private:

    Uint32 m_lastTicks;
    Uint32 m_fixedTicks;

    bool m_isFixed;
};