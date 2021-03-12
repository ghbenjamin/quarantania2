#pragma once


// Given a start and end value, provide a linear interpolation for the intermediate values.
template <typename T>
class LinearInterpolator
{
public:
    LinearInterpolator( T start, T end )
        : m_start(start), m_diff(end - start) {}
    LinearInterpolator()
        : m_start(T{}), m_diff(T{}) {}
    
    ~LinearInterpolator() = default;
    
    // Return the value at the given fraction ( 0 <= frac <= 1 )
    T value( float frac )
    {
        return m_start + (m_diff * frac);
    }
    
private:
    T m_start;
    T m_diff;
};

//template<>
//class LinearInterpolator<Colour>
//{
//public:
//    LinearInterpolator( Colour start, Colour end )
//            : m_start(start), m_diff(end - start) {}
//    LinearInterpolator()
//            : m_start(), m_diff() {}
//
//    ~LinearInterpolator() = default;
//
//    // Return the value at the given fraction ( 0 <= frac <= 1 )
//    Colour value( float frac )
//    {
//        return m_start + (m_diff * frac);
//    }
//
//private:
//    Colour m_start;
//    Colour m_diff;
//};



// Given a start and end value, linearly interpolate between the two values, taking the specified time to do it.
template <typename T>
class TimedLinearInterpolator
{
public:
    TimedLinearInterpolator( T start, T end, float seconds )
        : m_interpolator(start, end), m_maxSeconds(seconds), m_curr(start), m_currSeconds(0) {}
        
    TimedLinearInterpolator()
        : m_curr(T{}), m_currSeconds(0), m_maxSeconds(0) {}
        
    // Advance the current position of the interpolation by the given number of milliseconds
    void advance( std::uint32_t ticks )
    {
        m_currSeconds += (ticks / 1000.f);
        
        if (m_currSeconds >= m_maxSeconds)
        {
            m_finished = true;
            m_currSeconds = m_maxSeconds;
        }
        
        m_curr = m_interpolator.value( m_currSeconds / m_maxSeconds );
    }
    
    T const& current() const
    {
        return m_curr;
    }
    
    // Return true once the value has reached the endpoint.
    bool isFinished() const
    {
        return m_finished;
    }
    
private:
    
    LinearInterpolator<T> m_interpolator;
    T m_curr;
    float m_currSeconds;
    float m_maxSeconds;
    bool m_finished = false;
};