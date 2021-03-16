#pragma once

#include <array>
#include <graphics/Texture.h>

enum class RenderLayer
{
    Tiles   = 0,
    Entity  = 1,
    Actor   = 2,
    GFX     = 3,
    FOV     = 4,
    Overlay = 5,
    UI      = 6
};

inline const int RENDER_LAYER_COUNT = 7;

class Sprite;

class RenderObject
{
public:
    RenderObject(TextureHandle handle);
    ~RenderObject() = default;
    
    static constexpr int FLOATS_PER_QUAD = 48;
    static constexpr int TRIANGLES_PER_QUAD = 6;
    
    void setTextureVerts( int idx, float texX, float texY, float texW, float texH );
    void setScreenVerts( int idx, float scX, float scY, float scW, float scH );
    void setColourVerts( int idx, float r, float g, float b, float a );
    
    void merge( RenderObject other );
    int getDataSize();
    GLfloat* getData();
    TextureHandle getHandle() const;
    
    void setScreenPosition( int idx, Vector2f pos );
    void setScreenSize( int idx, Vector2f size );
    
    RectF const& getScreenBounds() const;
    
private:
    
    TextureHandle m_handle;
    std::vector<GLfloat> m_data;
    RectF m_screenBounds;
};