#pragma once

#include <array>
#include <graphics/Texture.h>
#include <graphics/Shader.h>

enum class RenderLayer
{
    Tiles   = 0,
    Entity,
    Actor,
    GFX,
    FOV,
    Overlay,
    UI
};

enum class ShaderType
{
    QuadShader = 0,
    TextShader = 1,
    ColourShader = 2,
};

inline const int RENDER_LAYER_COUNT = 7;

class Sprite;

class RenderObject
{
public:
    RenderObject(TextureHandle handle, ShaderType shader);
    RenderObject(TextureHandle handle);
    RenderObject();
    ~RenderObject() = default;
    
    static constexpr int FLOATS_PER_QUAD = 48;
    static constexpr int TRIANGLES_PER_QUAD = 6;
    
    // Set the uv coordinates of the texture vetex (in uv space)
    void setTextureVerts( int idx, float texX, float texY, float texW, float texH );
    
    // Set the coodinates of of the screen vertex (in screen space)
    void setScreenVerts( int idx, float scX, float scY, float scW, float scH );
    
    // Set the colour bytes for the vertex
    void setColourVerts( int idx, float r, float g, float b, float a );
    
    // Set only the alpha bytes for the vertex, leaving the other colours unchanged
    void setAlphaVerts( int idx, float a );
    
    // Add a new quad to the render object
    void addQuad( RectF screenOffsets, RectF uvBounds );
    
    // Merge another render object onto this one.
    void merge( RenderObject other );
    
    // Can the specified other render object be merged into this one?
    bool canMerge( RenderObject const& other ) const;

    void setScissor( RectI bounds );
    void removeScissor();


    // Data accessors
    
    int getDataSize();
    GLfloat* getData();
    TextureHandle getHandle() const;
    int getShaderType() const;
    RectI const& getScissor() const;
    
private:
    TextureHandle m_handle;
    ShaderType m_shader;
    std::vector<GLfloat> m_data;
    RectI m_scissor;
};