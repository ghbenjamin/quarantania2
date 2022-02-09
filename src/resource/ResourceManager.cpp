
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
    auto surfacePath = resourceRoot / "surfaces";
    auto ninepatchPath = resourceRoot / "ninepatch";
    auto shaderPath = resourceRoot / "shaders";
    auto uixmlPath = resourceRoot / "ui" / "defines";

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

    // Load all images
    for ( auto const& file : std::filesystem::directory_iterator(imgPath) )
    {
        auto const& fpath = file.path();
        if ( fpath.has_extension() && fpath.extension().string() == ".png" )
        {
            addImageResource( fpath.stem().string() );
        }
    }
    
    // Load all surfaces
    for ( auto const& file : std::filesystem::directory_iterator(surfacePath) )
    {
        auto const& fpath = file.path();
        if ( fpath.has_extension() && fpath.extension().string() == ".png" )
        {
            addSurfaceResource( fpath.stem().string() );
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
    
    // Load all UI XMI docs
    for ( auto const& file : std::filesystem::directory_iterator(uixmlPath) )
    {
        auto const& fpath = file.path();
        if ( fpath.has_extension() && fpath.extension().string() == ".xml" )
        {
            addXMLDoc( fpath.stem().string(), fpath.string() );
        }
    }

    // Load all shader programs
    // TBD maybe load these in externally
    
    addShaderProgram( "quad_shader", "simple_screenspace", "simple_sampler" );
    addShaderProgram( "text_shader", "simple_screenspace", "text_sampler" );
    addShaderProgram( "colour_only_shader", "simple_screenspace", "block_colour" );
    addShaderProgram( "no_projection", "no_projection", "simple_sampler" );
    addShaderProgram( "scene_fadeout", "no_projection", "scene_fadeout" );

    for ( auto const &[k, v] : m_spritesheets )
    {
        v->load();
    }

    for ( auto const &[k, v] : m_images )
    {
        v->load();
    }
    
    for ( auto const &[k, v] : m_surfaces )
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
    
    for (auto const &[k, v] : m_shaderProgs )
    {
        v->load();
    }
    
    for (auto const &[k, v] : m_xml )
    {
        v->load();
    }
}

void ResourceManager::unloadAll()
{
    m_spritesheets.clear();
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
        Logger::get().error( "Unknown sprite [{}]\n", key.toString() );
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
        Logger::get().error( "Unknown image [{}]\n", imgName );
        std::terminate();
    }
}

SurfacePtr ResourceManager::getSurface( std::string const &surfaceName )
{
    try
    {
        return m_surfaces.at(surfaceName)->get();
    }
    catch ( [[maybe_unused]] std::exception const& ex )
    {
        Logger::get().error( "Unknown surface [{}]\n", surfaceName );
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
        Logger::get().error( "Unknown image [{}]\n", imgName );
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
        Logger::get().error( "Unknown font [{}]\n", fname );
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
        Logger::get().error( "Unknown ninepatch [{}]\n", name );
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
        Logger::get().error( "Unknown shader [{}]\n", name );
        std::terminate();
    }
}

ShaderProgramResource& ResourceManager::getShaderProgram( std::string const &name )
{
    try
    {
        return *m_shaderProgs.at(name);
    }
    catch ( [[maybe_unused]] std::exception const& ex )
    {
        Logger::get().error( "Unknown shader program [{}]\n", name );
        std::terminate();
    }
}

SpritesheetResource const& ResourceManager::getSpritesheet( std::string const &sheetName )
{
    try
    {
        return *m_spritesheets.at(sheetName);
    }
    catch ( [[maybe_unused]] std::exception const& ex )
    {
        Logger::get().error( "Unknown spritesheet program [{}]\n", sheetName );
        std::terminate();
    }
}


TextResource &ResourceManager::getXMLDoc( std::string const &name )
{
    try
    {
        return *m_xml.at(name);
    }
    catch ( [[maybe_unused]] std::exception const& ex )
    {
        Logger::get().error( "Unknown xml doc program [{}]\n", name );
        std::terminate();
    }
}





std::shared_ptr<FtFontFace> ResourceManager::getDefaultFont()
{
    return getFont( getDefaultFontName() );
}

void ResourceManager::addImageResource(const std::string &name)
{
    m_images.emplace(name, std::make_shared<ImageResource>( name ));
}

void ResourceManager::addSurfaceResource( std::string const &name )
{
    m_surfaces.emplace( name, std::make_shared<SurfaceResource>( name ) );
}

void ResourceManager::addSpritesheetResource(const std::string &name)
{
    m_spritesheets.emplace(name, std::make_shared<SpritesheetResource>( name ));
}

void ResourceManager::addNinepatchResource(const std::string &name)
{
    m_patches.emplace(name, std::make_shared<NinePatchResource>( name ));
}

void ResourceManager::addShaderResource(std::string const& name, GLuint type)
{
    m_shaders.emplace(name, std::make_shared<ShaderResource>(name, type) );
}

void ResourceManager::addShaderProgram( std::string const &name, std::string const &vertName, std::string const &fragName )
{
    m_shaderProgs.emplace( name, std::make_shared<ShaderProgramResource>(name, vertName, fragName));
}

void ResourceManager::addXMLDoc( std::string const &name, std::string const &path )
{
    m_xml.emplace( name, std::make_shared<TextResource>(name, path));
}


const std::string ResourceManager::getDefaultFontName()
{
//    return "robotoslab-regular";
    return "inconsolata-regular";
}









