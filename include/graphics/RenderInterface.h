#pragma once

#include <graphics/RenderObject.h>
#include <graphics/Window.h>

class Camera;

class RenderInterface
{
public:
    RenderInterface( Renderer* renderer );
    virtual ~RenderInterface() = default;

    // The items which will be rendered to the screen at the end of the frame
    std::vector<RenderObject> const& renderables() const;

    void clear();
    void present();
    
    void addItem( RenderObject obj, RenderLayer layer );

    // Set the current camera
    void setCamera( Camera* camera );

private:
    Camera* m_camera;
    Renderer* m_renderer;
    std::vector<RenderObject> m_renderables;
};