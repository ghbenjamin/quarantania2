#include <game/Camera.h>
#include <utils/Assert.h>
#include <utils/Logging.h>

Camera::Camera()
: m_rect{0, 0, 0, 0}
{
}

void Camera::setPosition(Vector2i pos)
{
    m_position = pos.convert<float>();
    enforceBounds();
}

void Camera::moveBy(Vector2f delta)
{
    m_position += delta;
    enforceBounds();
}

void Camera::setBounds(Vector2i bounds)
{
    m_bounds = bounds;
    enforceBounds();
}

void Camera::setViewportSize(Vector2i size)
{
    m_size = size;
    enforceBounds();
}

void Camera::enforceBounds()
{
    if ( m_bounds.x() < m_size.x() )
    {
        m_position.x(0);
        m_internalOffset.x( (m_size.x() - m_bounds.x()) / 2 );
    }
    else
    {
        if ( m_position.x() < 0 )
            m_position.x(0);

        if ( m_position.x() + m_size.x() > m_bounds.x() )
            m_position.x( (float)(m_bounds.x() - m_size.x()) );
    }

    if ( m_bounds.y() < m_size.y() )
    {
        m_position.y(0);
        m_internalOffset.y( (m_size.y() - m_bounds.y()) /2 );
    }
    else
    {
        if ( m_position.y() < 0 )
            m_position.y(0);

        if ( m_position.y() + m_size.y() > m_bounds.y() )
            m_position.y( (float)(m_bounds.y() - m_size.y()) );
    }

    m_roundedPosition = m_position.convert<int>();
    m_rect = { m_roundedPosition.x(), m_roundedPosition.y(), m_size.x(), m_size.y() };
}

Vector2i const &Camera::getPosition() const
{
    return m_roundedPosition;
}

bool Camera::intersects(SDL_Rect* other) const 
{
    return SDL_HasIntersection( &m_rect, other ) == SDL_TRUE;
}

void Camera::setViewportScreenOffset(Vector2i offset)
{
    m_viewportScreenOffset = offset;
}

Vector2i const &Camera::getViewportScreenOffset() const
{
    return m_viewportScreenOffset;
}

void Camera::translate(SDL_Rect &spos) const
{
    spos.x -= m_roundedPosition.x();
    spos.y -= m_roundedPosition.y();
    spos.x += m_internalOffset.x();
    spos.y += m_internalOffset.y();
}

