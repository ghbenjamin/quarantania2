#pragma once

#include <array>
#include <graphics/Texture.h>

enum class RenderLayer
{
    Begin = 0,

    Tiles = 1,
    Entity = 2,
    Actor = 3,
    GFX = 4,
    FOV = 5,
    OVERLAY_UI = 6,
    UI = 7,

    End = 8
};

class Sprite;

class RenderObject
{
public:
    RenderObject(TextureHandle handle);
    ~RenderObject() = default;
    
    void setScreenPosition( Vector2f pos );
    void setScreenSize( Vector2f size );
    
    void setTextureVerts( float texX, float texY, float texW, float texH );
    void setScreenVerts( float scX, float scY, float scW, float scH );
    
    void setRenderLayer( RenderLayer layer );
    
    RectF const& getScreenBounds() const;
    TextureHandle getHandle() const;
    std::array<GLfloat, 24> const& getVerts() const;
    
    bool operator<( RenderObject const &rhs ) const;

private:
    
    RenderLayer m_renderLayer;
    TextureHandle m_handle;
    std::array<GLfloat, 24> m_verts;
    RectF m_screenBounds;
};