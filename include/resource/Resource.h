#pragma once

#include <memory>
#include <string>
#include <resource/Texture.h>
#include <resource/Font.h>
#include <resource/Spritesheet.h>

class ResourceManager;

enum class ResourceType
{
    Null,
    Image,
    Spritesheet,
    Font,
    Audio
};

class Resource
{
public:
    Resource( std::string const& key, std::string const& path );

    virtual ~Resource() = default;
    Resource( const Resource& ) = delete;
    Resource& operator=( const Resource& ) = delete;

    virtual void load(  ) = 0;
    virtual void unload() = 0;

    std::string const& getKey() const;
    std::string const& getPath() const;

    bool isLoaded() const;
    ResourceType getType() const;

protected:

    std::string m_key;
    std::string m_path;

    bool m_isLoaded;
    ResourceType m_type;

};

class ImageResource : public Resource
{
public:
    ImageResource(std::string const &key, std::string const &path);
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
    FontResource(std::string const &key, std::string const &path, int fontSize);
    ~FontResource() override = default;

    const FontPtr &get() const;
    void load() override;
    void unload() override;

private:
    FontPtr m_font;
    int m_fontSize;
};

class SpritesheetResource : public Resource
{
public:
    SpritesheetResource(std::string const &key, std::string const &path);
    ~SpritesheetResource() override = default;

    const SpritesheetPtr &get() const;
    void load() override;
    void unload() override;

private:
    SpritesheetPtr m_spritesheet;
};
