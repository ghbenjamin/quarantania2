#pragma once

#include <type_traits>
#include <vector>

template <typename T,
    typename = typename std::enable_if<std::is_arithmetic<T>::value, T>::type>
class IdPool
{
public:

    IdPool( ) : m_next(1) {}
    explicit IdPool( T begin ) : m_next(begin) {}
    ~IdPool() = default;

    T next()
    {
        if ( !m_returned.empty() )
        {
            T back = m_returned.back();
            m_returned.pop_back();
            return back;
        }
        else
        {
            return m_next++;
        }
    }

    void release( T t )
    {
        m_returned.push_back(t);
    }


private:
    T m_next;
    std::vector<T> m_returned;
};