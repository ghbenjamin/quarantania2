#pragma once

#include <memory>
#include <string>
#include <array>

#include <graphics/Texture.h>
#include <resource/Font.h>
#include <resource/Spritesheet.h>

class ResourceManager;

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



class FontResource : public Resource
{
public:
    FontResource(std::string const &name);
    ~FontResource() override = default;

    const FontDataPtr &get(int fontSize);
    void load() override;
    void unload() override;

private:
    void loadForSize(int fontSize);

    std::unordered_map<int, FontDataPtr> m_fonts;
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

private:
    int m_borderWidth;
    TexturePtr m_texture;
    std::array<RectI, 9> m_offsets;
};
