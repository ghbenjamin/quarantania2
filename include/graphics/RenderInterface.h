#pragma once

#include <graphics/RenderObject.h>
#include <graphics/Window.h>

class Camera;

class RenderInterface
{
public:
    explicit RenderInterface() ;
    virtual ~RenderInterface() = default;

    // The items which will be rendered to the screen at the end of the frame
    std::vector<RenderObject> const& renderables() const;

    void clear();
    void present();

    // Add an item which will be rendered relative to the position of the current camera
    void addWorldItem(RenderObject obj);

    // Add an item which will be rendered without taking the position of the camera into account
    void addScreenItem(RenderObject obj);

    // Set the current camera
    void setCamera( Camera* camera );

private:
    Camera* m_camera;
    std::vector<RenderObject> m_renderables;
};