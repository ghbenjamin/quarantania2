#pragma once

#include <vector>
#include <unordered_map>
#include <resource/Manifest.h>
#include <resource/Texture.h>
#include <resource/Spritesheet.h>
#include <resource/Resource.h>
#include <resource/Font.h>
#include <graphics/Window.h>
#include <utils/Assert.h>

class ResourceManager
{
public:

    static ResourceManager& get()
    {
        static ResourceManager instance;
        return instance;
    }

    ResourceManager( const ResourceManager& ) = delete;
    ResourceManager& operator=( const ResourceManager& ) = delete;
    ~ResourceManager() = default;

    void setWindow( WindowPtr const& wnd );
    WindowPtr const& getWindow() const;

    void loadAll();
    void unloadAll();

    void registerAll(std::string const &manifest);

    template <typename RType, typename ... Args>
    void addResource( Args... args )
    {
        auto dPtr = std::make_shared<RType>(std::forward<Args>(args)...);
        auto bPtr = std::static_pointer_cast<Resource>(dPtr);
        m_resources.emplace( bPtr->getKey(), bPtr );
    }

    template <typename RType>
    std::shared_ptr<RType> const& getResource( std::string const& key )
    {
        auto it = m_resources.find(key);

        AssertMsg(it != m_resources.end(), ("Missing resource: " + key).c_str());

        auto bPtr = it->second;
        auto dPtr = std::static_pointer_cast<RType>(bPtr);
        return std::move(dPtr);
    }

    // Shortcuts

    [[nodiscard]]
    Sprite getSprite( std::string const& sheet, std::string const& name );

    [[nodiscard]]
    Sprite getSprite( SpritesheetKey const& key );

    [[nodiscard]]
    Sprite getSprite( std::string const& imgName );

    [[nodiscard]]
    FontPtr getFont( std::string const& fname );

private:

    ResourceManager() = default;
    ManifestData readResourceManifest( std::string const& path);

private:
    std::unordered_map<std::string, std::shared_ptr<Resource>> m_resources;
    WindowPtr m_context;

};