#pragma once

#include <graphics/RenderObject.h>
#include <graphics/Window.h>

class Camera;

class RenderInterface
{
public:
    explicit RenderInterface( WindowCPtr ) ;
    virtual ~RenderInterface() = default;

    std::vector<RenderObject> const& renderables() const;
    WindowCPtr const& window() const;

    void clear();
    void present();

    void addWorldItem(RenderObject obj);
    void addScreenItem(RenderObject obj);

    void setCamera( Camera* camera );

private:

    Camera* m_camera;

    std::vector<RenderObject> m_renderables;
    WindowCPtr m_window;
};