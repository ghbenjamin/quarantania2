#pragma once

#include "SDL.h"

class Timer
{
public:
    Timer();
    ~Timer();

    void start();
    void stop();

    uint32_t elapsed();

private:

    uint32_t m_lastTicks;
    uint32_t m_fixedTicks;

    bool m_isFixed;
};