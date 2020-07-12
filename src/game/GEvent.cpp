#include <game/GEvent.h>

GEventId GEventBase::m_globalId = 0;

bool GEventCallback::operator<(const GEventCallback &rhs) const
{
    if (target < rhs.target)
        return true;
    if (rhs.target < target)
        return false;
    return timing < rhs.timing;
}

bool GEventCallback::operator>(const GEventCallback &rhs) const
{
    return rhs < *this;
}

bool GEventCallback::operator<=(const GEventCallback &rhs) const
{
    return !(rhs < *this);
}

bool GEventCallback::operator>=(const GEventCallback &rhs) const
{
    return !(*this < rhs);
}
