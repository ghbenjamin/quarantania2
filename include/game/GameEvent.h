#pragma once

#include <unordered_map>
#include <queue>

#include <utils/Logging.h>

using GEventId = std::size_t;

enum class GEventTiming
{
    Before,
    On,
    After
};

static constexpr int GEventTimingCount = 3;


class GEventBase
{
public:
    GEventBase() = default;
    virtual ~GEventBase() = default;

protected:
    static GEventId m_globalId;
};

template <typename ET>
class GameEvent : public GEventBase
{
public:

    GameEvent() = default;
    virtual ~GameEvent() = default;

    static GEventId id()
    {
        static GEventId m_id = m_globalId++;
        return m_id;
    }
};

class GEventSubBase
{
public:
    GEventSubBase() = default;
    virtual ~GEventSubBase() = default;
};

template <typename ET>
class GEventSub : virtual public GEventSubBase
{
public:
    GEventSub() = default;
    ~GEventSub() override = default;

    virtual void accept( ET* evt ) = 0;
};

struct GEventCallback
{
    GEventSubBase* target;
    GEventTiming timing;

    bool operator<(const GEventCallback &rhs) const;
    bool operator>(const GEventCallback &rhs) const;
    bool operator<=(const GEventCallback &rhs) const;
    bool operator>=(const GEventCallback &rhs) const;
};