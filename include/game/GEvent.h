#pragma once

#include <unordered_map>
#include <queue>

#include <utils/Logging.h>

using GEventId = std::size_t;

class GEventBase
{
public:
    GEventBase() = default;
    virtual ~GEventBase() = default;

protected:
    static GEventId m_globalId;
};

template <typename ET>
class GEvent : public GEventBase
{
public:

    GEvent() = default;
    virtual ~GEvent() = default;

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

class GEventHub
{
public:
    GEventHub() = default;
    ~GEventHub() = default;

    GEventHub( const GEventHub& ) = delete;
    GEventHub& operator=( const GEventHub& ) = delete;

    template <typename EvtType, typename SubType>
    void subscribe( SubType* receiver )
    {
        static_assert( std::is_base_of_v<GEventSubBase, SubType> );
        static_assert( std::is_base_of_v<GEventBase, EvtType> );

        GEventSubBase* base = static_cast<SubType*>(receiver);

        m_subs.emplace( GEvent<EvtType>::id(), base );
    };

    template <typename EvtType, typename SubType>
    void unsubscribe( SubType* receiver )
    {
        static_assert( std::is_base_of_v<GEventSubBase, SubType> );
        static_assert( std::is_base_of_v<GEventBase, EvtType> );

        GEventSubBase* base = receiver;
        auto it_range = m_subs.equal_range( GEvent<EvtType>::id() );

        for (auto it = it_range.first; it != it_range.second; it++)
        {
            if ( it->second == base )
            {
                m_subs.erase(it);
                break;
            }
        }

    };

    template <typename EvtType>
    void broadcast( EvtType* evt )
    {
        static_assert( std::is_base_of_v<GEventBase, EvtType> );

        auto it_range = m_subs.equal_range( GEvent<EvtType>::id() );

        for (auto it = it_range.first; it != it_range.second; it++)
        {
            GEventSubBase* base = it->second;
            dynamic_cast<GEventSub<EvtType>*>(base)->accept( evt );
        }
    }

    template <typename EvtType, typename... Args>
    void broadcast( Args... args )
    {
        static_assert( std::is_base_of_v<GEventBase, EvtType> );

        EvtType evt { std::forward<Args>(args)... };

        broadcast( &evt );
    }


private:

    std::unordered_multimap<GEventId, GEventSubBase*> m_subs;
};


