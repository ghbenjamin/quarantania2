#include <game/Camera.h>
#include <utils/Assert.h>

Camera::Camera()
: m_size{0, 0}, m_bounds{0, 0}, m_position{0, 0}, m_roundedPosition{0, 0}
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
    Assert( m_bounds.x() >= m_size.x() );
    Assert( m_bounds.y() >= m_size.y() );

    if ( m_position.x() < 0 )
        m_position.x(0);

    if ( m_position.y() < 0 )
        m_position.y(0);

    if ( m_position.x() + m_size.x() > m_bounds.x() )
        m_position.x( (float)(m_bounds.x() - m_size.x()) );

    if ( m_position.y() + m_size.y() > m_bounds.y() )
        m_position.y( (float)(m_bounds.y() - m_size.y()) );

    m_roundedPosition = m_position.convert<int>();
    m_rect = { m_roundedPosition.x(), m_roundedPosition.y(), m_bounds.x(), m_bounds.y() };
}

Vector2i const &Camera::getPosition() const
{
    return m_roundedPosition;
}

bool Camera::intersects(SDL_Rect* other)
{
    return SDL_HasIntersection( &m_rect, other ) == SDL_TRUE;
}
