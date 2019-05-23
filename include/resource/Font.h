#pragma once

#include <memory>
#include <string>
#include <SDL2/SDL_ttf.h>

class Font;
using FontPtr = std::shared_ptr<Font>;

class Font
{
public:

    explicit Font( TTF_Font* ptr );
    ~Font();

    Font( const Font& ) = delete;
    Font& operator=( const Font& ) = delete;

    static FontPtr loadFont( std::string const& path, int size );

private:

    TTF_Font* m_font;
};