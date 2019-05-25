#pragma once

#include <utils/Containers.h>

class Camera
{
public:

    Camera();
    ~Camera() = default;

    void setPosition( Vector2i pos );
    void moveBy( Vector2f delta );

    void setBounds( Vector2i bounds );
    void setViewportSize( Vector2i size );

    Vector2i const& getPosition() const;

private:

    void enforceBounds();

    Vector2i m_bounds;
    Vector2i m_size;

    Vector2f m_position;
    Vector2i m_roundedPosition;
};