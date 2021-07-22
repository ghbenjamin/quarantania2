#pragma once

#include <optional>
#include <graphics/RenderObject.h>

class Renderer;

class RenderInterface
{
public:
    RenderInterface( Renderer* renderer );
    ~RenderInterface() = default;

    // Add an item to the specified render queue
    void addItem( RenderObject obj, RenderLayer layer );

    // Stop clearing the specified render queue at the end of each frame, and keep rendering
    // whatever is currently in the buffer
    void holdRenderQueue( RenderLayer layer );

    // Release the hold on the current render queue and start clearing the buffer at the end
    // of each frame again
    void releaseRenderQueue( RenderLayer layer );
    
    // Set the level camera offset. This should be called once at the beginning of each frame
    void setCameraOffset( Vector2f offset );

    void setScissor(RectI scissor);
    void removeScissor();
    
    
    void startFadeout( float time );
    void startFadein( float time );
    
    

private:
    Renderer* m_renderer;
    
    std::optional<RectI> m_currentScissor;
};