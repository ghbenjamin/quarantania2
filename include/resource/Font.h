#pragma once

#include <memory>
#include <string>
#include <SDL2/SDL_ttf.h>
#include <utils/Colour.h>

class Font;
class Texture;
using FontPtr = std::shared_ptr<Font>;

class Font
{
public:

    static FontPtr loadFont( std::string const& path, int size );

    explicit Font( TTF_Font* ptr );
    ~Font();

    Font( const Font& ) = delete;
    Font& operator=( const Font& ) = delete;

    std::shared_ptr<Texture> renderText( std::string const& text, Colour colour=Colour::Black, int wrapWidth=-1 );

private:

    TTF_Font* m_font;
};