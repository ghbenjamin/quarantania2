
#include <iostream>
#include <filesystem>

#include <resource/ResourceManager.h>
#include <utils/Logging.h>

#include <graphics/Text.h>

void ResourceManager::setWindow(WindowPtr const &wnd)
{
    m_context = wnd;
}

WindowPtr const &ResourceManager::getWindow() const
{
    return m_context;
}

void ResourceManager::loadAll()
{
    auto resourceRoot = std::filesystem::path( "../resource/" );
    auto spritesheetPath = resourceRoot / "spritesheet";
    auto fontPath = resourceRoot / "font";
    auto imgPath = resourceRoot / "img";
    auto ninepatchPath = resourceRoot / "ninepatch";
    auto shaderPath = resourceRoot / "shaders";

    // Load all spritesheets
    for ( auto const& file : std::filesystem::directory_iterator(spritesheetPath) )
    {
        auto const& fpath = file.path();
        if ( fpath.has_extension() && fpath.extension().string() == ".png" )
        {
            auto jsonPath = fpath;
            jsonPath.replace_extension(".json");

            if ( std::filesystem::exists(jsonPath) )
            {
                addSpritesheetResource( fpath.stem().string() );
            }
        }
    }

    // Load all patches
    for ( auto const& file : std::filesystem::directory_iterator(ninepatchPath) )
    {
        auto const& fpath = file.path();
        if ( fpath.has_extension() && fpath.extension().string() == ".png" )
        {
            auto jsonPath = fpath;
            jsonPath.replace_extension(".json");

            if ( std::filesystem::exists(jsonPath) )
            {
                addNinepatchResource( fpath.stem().string() );
            }
        }
    }

    // Load all fonts
    for ( auto const& file : std::filesystem::directory_iterator(fontPath) )
    {
        auto const& fpath = file.path();
        if ( fpath.has_extension() && fpath.extension().string() == ".ttf" )
        {
            addFontResource( fpath.stem().string() );
        }
    }

    // Load all images
    for ( auto const& file : std::filesystem::directory_iterator(imgPath) )
    {
        auto const& fpath = file.path();
        if ( fpath.has_extension() && fpath.extension().string() == ".png" )
        {
            addImageResource( fpath.stem().string() );
        }
    }
    
    // Load all shaders
    for ( auto const& file : std::filesystem::directory_iterator(shaderPath) )
    {
        auto const& fpath = file.path();
        if ( fpath.has_extension()  )
        {
            if ( fpath.extension().string() == ".frag" )
            {
                addShaderResource( fpath.stem().string(), GL_FRAGMENT_SHADER );
            }
            else if ( fpath.extension().string() == ".vert" )
            {
                addShaderResource( fpath.stem().string(), GL_VERTEX_SHADER );
            }
        }
    }


    for ( auto const &[k, v] : m_spritesheets )
    {
        v->load();
    }

    for ( auto const &[k, v] : m_images )
    {
        v->load();
    }

    for ( auto const &[k, v] : m_fonts )
    {
        v->load();
    }

    for ( auto const &[k, v] : m_patches )
    {
        v->load();
    }
    
    for (auto const &[k, v] : m_shaders )
    {
        v->load();
    }
    
//
//    FtFontManager manager;
//
//    auto face = manager.loadFontFace( "inconsolata-regular", 20 );
//    int i = 1;
}

void ResourceManager::unloadAll()
{
    m_spritesheets.clear();
    m_fonts.clear();
    m_images.clear();
    m_patches.clear();
}

Sprite ResourceManager::getSprite(std::string const &sheet, std::string const &name)
{
    return getSprite({sheet, name});
}

Sprite ResourceManager::getSprite(SpritesheetKey const& key)
{
    try
    {
        return m_spritesheets.at(key.sheetName)->get()->getSprite(key);
    }
    catch ( [[maybe_unused]] std::exception const& ex )
    {
        Logging::log( "ERROR: Unknown sprite [{}, {}, {}]\n", key.spriteName, key.gid, key.sheetName );
        std::terminate();
    }
}

Sprite ResourceManager::getImageAsSprite(std::string const& imgName)
{
    try
    {
        return m_images.at(imgName)->getSprite();
    }
    catch ( [[maybe_unused]] std::exception const& ex )
    {
        Logging::log( "ERROR: Unknown image [{}]\n", imgName );
        std::terminate();
    }
}

TexturePtr ResourceManager::getTexture(const std::string &imgName)
{
    try
    {
        return m_images.at(imgName)->get();
    }
    catch ( [[maybe_unused]] std::exception const& ex )
    {
        Logging::log( "ERROR: Unknown image [{}]\n", imgName );
        std::terminate();
    }
}


std::shared_ptr<FtFontFace> ResourceManager::getFont(std::string const &fname )
{
    try
    {
        return m_fontManager.getFont( fname );
    }
    catch ( [[maybe_unused]] std::exception const& ex )
    {
        Logging::log( "ERROR: Unknown font [{}]\n", fname );
        std::terminate();
    }
}


NinePatchResource const& ResourceManager::getNinePatch(const std::string &name)
{
    try
    {
        return *m_patches.at(name);
    }
    catch ( [[maybe_unused]] std::exception const& ex )
    {
        Logging::log( "ERROR: Unknown ninepatch [{}]\n", name );
        std::terminate();
    }
}

ShaderResource const& ResourceManager::getShader( const std::string &name)
{
    try
    {
        return *m_shaders.at(name);
    }
    catch ( [[maybe_unused]] std::exception const& ex )
    {
        Logging::log( "ERROR: Unknown shader [{}]\n", name );
        std::terminate();
    }
}

//std::shared_ptr<FtFontFace> ResourceManager::getFont2( std::string name, int fontSize )
//{
//    return m_fontManager.getFont( name, fontSize );
//}


std::shared_ptr<FtFontFace> ResourceManager::getDefaultFont()
{
    return getFont( getDefaultFontName() );
}


void ResourceManager::addImageResource(const std::string &name)
{
    m_images.emplace(name, std::make_shared<ImageResource>( name ));
}

void ResourceManager::addSpritesheetResource(const std::string &name)
{
    m_spritesheets.emplace(name, std::make_shared<SpritesheetResource>( name ));
}

void ResourceManager::addFontResource(const std::string &name)
{
    m_fonts.emplace(name, std::make_shared<FontResource>( name ));
}

void ResourceManager::addNinepatchResource(const std::string &name)
{
    m_patches.emplace(name, std::make_shared<NinePatchResource>( name ));
}

void ResourceManager::addShaderResource(std::string const& name, GLuint type)
{
    m_shaders.emplace(name, std::make_shared<ShaderResource>(name, type) );
}

const std::string ResourceManager::getDefaultFontName()
{
    return "inconsolata-regular";
}



