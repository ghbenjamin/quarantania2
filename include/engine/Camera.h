#pragma once

#include <utils/Containers.h>

class RenderObject;

class Camera
{
public:

    Camera( Vector2i bounds );
    ~Camera() = default;

    void setPosition( Vector2i pos );
    void moveBy( Vector2f delta );

    // Set the size of our current viewport
    void setViewportSize( Vector2i size );

    // Get/set the (screen) position of our viewport
    void setViewportScreenOffset( Vector2i offset );
    Vector2i const& getViewportScreenOffset() const;

    // Return the world coordinates of the camera
    Vector2i const& getPosition() const;
    
    // Return the vector needed to translate world space into camera space
    Vector2f getOffset() const;

    Vector2i screenToWorld( Vector2i const& coords ) const;
    Vector2i worldToScreen( Vector2i const& coords ) const;

    // Set the scroll speed as a ratio of milliseconds to pixels (default 0.75f)
    void setScrollSpeed(float scrollSpeed);

    void centreOnWorldPosition( Vector2i worldPos );
    void centreOnTile( Vector2i tilePos );

private:

    // Clamp the position of the camera to the bounds of the world space
    void enforceBounds();

    // The world size of the map
    Vector2i m_bounds;

    // The screen size of the viewport into the map
    Vector2i m_size;

    // Our position, relative to the top-left of the world
    Vector2f m_position;
    Vector2i m_roundedPosition;

    Vector2i m_viewportScreenOffset;
    Vector2f m_internalOffset;

    float m_scrollSpeed;
};