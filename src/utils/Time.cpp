#include "utils/Time.h"

Timer::Timer()
  : m_fixedTicks(0),
    m_lastTicks(0),
    m_isFixed(false)
{
}

Timer::~Timer() = default;

void Timer::start()
{
    m_isFixed = false;
    m_fixedTicks = 0;
    m_lastTicks = SDL_GetTicks();
}

void Timer::stop()
{
    m_fixedTicks = SDL_GetTicks() - m_lastTicks;
    m_isFixed = true;
}

uint32_t Timer::elapsed()
{
    if (m_isFixed)
    {
        return m_fixedTicks;
    }
    else
    {
        return SDL_GetTicks() - m_lastTicks; 
    }
}