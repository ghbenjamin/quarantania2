#pragma once

#include <array>
#include <graphics/Texture.h>
#include <graphics/Shader.h>

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
    ~RenderObject() = default;
    
    static constexpr int FLOATS_PER_QUAD = 48;
    static constexpr int TRIANGLES_PER_QUAD = 6;
    
    void setTextureVerts( int idx, float texX, float texY, float texW, float texH );
    void setScreenVerts( int idx, float scX, float scY, float scW, float scH );
    void setColourVerts( int idx, float r, float g, float b, float a );
    
    void addQuad( RectF screenOffsets, RectF uvBounds );
    
    void merge( RenderObject other );
    int getDataSize();
    GLfloat* getData();
    TextureHandle getHandle() const;
    int getShaderType() const;
    
private:
    TextureHandle m_handle;
    ShaderType m_shader;
    std::vector<GLfloat> m_data;
};