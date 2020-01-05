#pragma once

#include <utils/Containers.h>

class RectIRange
{
public:

    class Iterator
    {
    public:
        using value_type = Vector2i;
        using reference = Vector2i&;
        using iterator_category=std::input_iterator_tag;
        using pointer = Vector2i*;
        using difference_type = void;

        Iterator(RectIRange* range, Vector2i current);
        ~Iterator() = default;

        bool operator==(Iterator const& i);
        bool operator!=(Iterator const& i);
        Iterator& operator++();
        Vector2i operator*();
        Vector2i* operator->();

    private:
        const RectIRange* m_range;
        Vector2i m_current;
    };

    RectIRange( RectI const& rect );
    ~RectIRange() = default;

    Iterator begin();
    Iterator end();

    RectI const& rect() const;

private:
    RectI m_rect;
    Vector2i m_begin;
    Vector2i m_end;
};