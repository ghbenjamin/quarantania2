#include <engine/Camera.h>
#include <utils/Assert.h>
#include <utils/Logging.h>
#include <utils/GlobalConfig.h>
#include <graphics/RenderObject.h>

Camera::Camera( Vector2i bounds )
: m_bounds(bounds), m_rect{0, 0, 0, 0}, m_scrollSpeed{0.75f}
{
}

void Camera::setPosition(Vector2i pos)
{
    m_position = pos.convert<float>();
    enforceBounds();
}

void Camera::moveBy(Vector2f delta)
{
    m_position += ( delta * m_scrollSpeed );
    enforceBounds();
}

void Camera::setViewportSize(Vector2i size)
{
    m_size = size;
    enforceBounds();
}

void Camera::enforceBounds()
{
    m_internalOffset = {0, 0};

    float overflowX = (float) m_size.x() / 3;
    float overflowY = (float) m_size.y() / 3;

    if ( m_position.x() < -overflowX )
        m_position.x(-overflowX);

    if ( m_position.x() + m_size.x() > m_bounds.x() + overflowX )
        m_position.x( (float)(m_bounds.x() - m_size.x()) + overflowX );

    if ( m_position.y() < -overflowY )
        m_position.y(-overflowY);

    if ( m_position.y() + m_size.y() > m_bounds.y() + overflowY )
        m_position.y( (float)(m_bounds.y() - m_size.y()) + overflowY );

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

void Camera::translate(RenderObject &spos) const
{
    spos.screenBounds.x(spos.screenBounds.x() + m_internalOffset.x() - m_roundedPosition.x());
    spos.screenBounds.y(spos.screenBounds.y() + m_internalOffset.y() - m_roundedPosition.y());
}

void Camera::setScrollSpeed(float scrollSpeed)
{
    m_scrollSpeed = scrollSpeed;
}

Vector2i Camera::worldToScreen(Vector2i const &coords)
{
    return {
        coords.x() - m_roundedPosition.x() + m_internalOffset.x(),
        coords.y() - m_roundedPosition.y() + m_internalOffset.y()
    };
}

Vector2i Camera::screenToWorld(Vector2i const &coords)
{
    return {
        coords.x() + m_roundedPosition.x() - m_internalOffset.x(),
        coords.y() + m_roundedPosition.y() - m_internalOffset.y()
    };
}

void Camera::centreOnWorldPosition(Vector2i worldPos)
{
    setPosition({
        worldPos.x() - m_bounds.x() / 2,
        worldPos.y() - m_bounds.y() / 2,
    });
}

void Camera::centreOnTile(Vector2i tilePos)
{
    centreOnWorldPosition({
        tilePos.x() * GlobalConfig::TileSizePx + GlobalConfig::TileSizePx * 2,
        tilePos.y() * GlobalConfig::TileSizePx + GlobalConfig::TileSizePx * 2,
    });
}

