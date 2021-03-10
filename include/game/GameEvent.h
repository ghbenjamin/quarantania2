#pragma once

#include <unordered_map>
#include <queue>

#include <utils/Logging.h>
#include <game/GameEventVariant.h>


using GEventId = std::size_t;

enum class GEventTiming
{
    Before = 0,
    On = 1,
    After = 2
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

    virtual void accept( GameEventVariant& evt ) = 0;

};


template <typename T>
class GameEventSub : public GEventSubBase
{
public:
    void accept( GameEventVariant& evt ) override
    {
        auto* ptr = static_cast<T*>(this);
        std::visit(*ptr, evt);
    }
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