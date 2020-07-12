#include <utils/Ranges.h>

RectIRange::Iterator::Iterator(RectIRange *range, Vector2i current)
: m_range(range), m_current(current)
{
}

bool RectIRange::Iterator::operator==(const RectIRange::Iterator &i)
{
    return m_range == i.m_range && m_current == i.m_current;
}

bool RectIRange::Iterator::operator!=(const RectIRange::Iterator &i)
{
    return !(*this == i);
}

RectIRange::Iterator &RectIRange::Iterator::operator++()
{
    if ( m_current.x() + 1 >= m_range->rect().x() + m_range->rect().w() )
    {
        m_current = { m_range->rect().x(), m_current.y() + 1 };
    }
    else
    {
        m_current = { m_current.x() + 1, m_current.y() };
    }

    return *this;
}

Vector2i RectIRange::Iterator::operator*()
{
    return m_current;
}

Vector2i* RectIRange::Iterator::operator->()
{
    return &m_current;
}

RectIRange::RectIRange(RectI const &rect)
:   m_rect(rect),
    m_begin({rect.x(), rect.y()}),
    m_end({rect.x(), rect.y() + rect.h()})
{
}

RectI const &RectIRange::rect() const
{
    return m_rect;
}

RectIRange::Iterator RectIRange::begin()
{
    return RectIRange::Iterator(this, m_begin);
}

RectIRange::Iterator RectIRange::end()
{
    return RectIRange::Iterator(this, m_end);
}
