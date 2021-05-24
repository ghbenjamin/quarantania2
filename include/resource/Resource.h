#pragma once

#include <memory>
#include <string>
#include <array>

#include <graphics/Texture.h>
#include <resource/Spritesheet.h>
#include <ui/lib/Background.h>
#include <graphics/Text.h>

class ResourceManager;
class ShaderProgram;
namespace UI { class NinePatch; }

class Resource
{
public:
    Resource( std::string const& name );

    virtual ~Resource() = default;
    Resource( const Resource& ) = delete;
    Resource& operator=( const Resource& ) = delete;

    virtual void load() = 0;
    virtual void unload() = 0;

    bool isLoaded() const;

protected:

    std::string m_name;
    bool m_isLoaded;
};

class ImageResource : public Resource
{
public:
    ImageResource(std::string const& name);
    ~ImageResource() override = default;

    const TexturePtr &get() const;
    Sprite getSprite() const;
    void load() override;
    void unload() override;

private:
    TexturePtr m_texture;
};


class SurfaceResource : public Resource
{
public:
    SurfaceResource(std::string const& name);
    ~SurfaceResource() override = default;
    
    const SurfacePtr &get() const;
    void load() override;
    void unload() override;

private:
    SurfacePtr m_surface;
};


class SpritesheetResource : public Resource
{
public:
    SpritesheetResource(std::string const &name);
    ~SpritesheetResource() override = default;

    const SpritesheetPtr &get() const;
    void load() override;
    void unload() override;

private:
    SpritesheetPtr m_spritesheet;
};


class NinePatchResource : public Resource
{
public:
    NinePatchResource(std::string const &name);
    ~NinePatchResource() override = default;

    void load() override;
    void unload() override;

    int borderWidth() const;
    TexturePtr const& texture() const;
    std::array<RectI, 9> offsets() const;

    UI::NinePatch getPatch() const;

private:
    int m_borderWidth;
    TexturePtr m_texture;
    std::array<RectI, 9> m_offsets;
};


class ShaderResource : public Resource
{
public:
    ShaderResource( std::string const& name, GLuint type);
    ~ShaderResource() override = default;
    
    void load() override;
    void unload() override;
    
    GLuint getType() const;
    GLuint getHandle() const;
    
private:
    GLuint m_type;
    GLuint m_handle;
};


class ShaderProgramResource : public Resource
{
public:
    ShaderProgramResource( std::string const& name, std::string const& vertName, std::string const& fragName );
    ~ShaderProgramResource() override = default;
    
    void load() override;
    void unload() override;
    
    std::shared_ptr<ShaderProgram> getProgram();
    GLuint getHandle() const;
    
private:
    std::shared_ptr<ShaderProgram> m_program;
    std::string m_vertName;
    std::string m_fragName;
};