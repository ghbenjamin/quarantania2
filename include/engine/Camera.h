#pragma once

#include <utils/Containers.h>

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

    // Does the specified (world) rect intersect the world rect of the camera?
    bool intersects( SDL_Rect* other ) const;
    void translate( SDL_Rect &spos ) const;

    Vector2i screenToWorld( Vector2i const& coords );
    Vector2i worldToScreen( Vector2i const& coords );

    void setScrollSpeed(float scrollSpeed);

    void centreOnWorldPosition( Vector2i worldPos );
    void centreOnTile( Vector2i tilePos );

private:

    void enforceBounds();

    // The world size of the map
    Vector2i m_bounds;

    // The screen size of the viewport into the map
    Vector2i m_size;

    // Our position, relative to the top-left of the world
    Vector2f m_position;
    Vector2i m_roundedPosition;

    // Cached (world) position rect for viewport culling
    SDL_Rect m_rect;

    Vector2i m_viewportScreenOffset;
    Vector2i m_internalOffset;

    float m_scrollSpeed;
};