#include <engine/Camera.h>
#include <utils/Assert.h>
#include <utils/Logging.h>
#include <utils/GlobalConfig.h>
#include <graphics/RenderObject.h>

Camera::Camera( Vector2i bounds )
: m_bounds(bounds), m_scrollSpeed{0.75f} {}

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
    m_internalOffset = {0.0f, 0.0f};

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
}

Vector2i const &Camera::getPosition() const
{
    return m_roundedPosition;
}

void Camera::setViewportScreenOffset(Vector2i offset)
{
    m_viewportScreenOffset = offset;
}

Vector2i const &Camera::getViewportScreenOffset() const
{
    return m_viewportScreenOffset;
}

Vector2f Camera::getOffset() const
{
    return m_internalOffset - m_position.convert<float>();
}


void Camera::setScrollSpeed(float scrollSpeed)
{
    m_scrollSpeed = scrollSpeed;
}

Vector2i Camera::worldToScreen(Vector2i const &coords) const
{
    return {
        coords.x() - m_roundedPosition.x() + (int) m_internalOffset.x(),
        coords.y() - m_roundedPosition.y() + (int) m_internalOffset.y()
    };
}

Vector2i Camera::screenToWorld(Vector2i const &coords) const
{
    return {
        coords.x() + m_roundedPosition.x() - (int) m_internalOffset.x(),
        coords.y() + m_roundedPosition.y() - (int) m_internalOffset.y()
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


