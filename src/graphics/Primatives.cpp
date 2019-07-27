#include <graphics/Primatives.h>
#include <resource/ResourceManager.h>

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    static constexpr int sdl_rmask = 0xff000000;
    static constexpr int sdl_gmask = 0x00ff0000;
    static constexpr int sdl_bmask = 0x0000ff00;
    static constexpr int sdl_amask = 0x000000ff;
#else
    static constexpr Uint32 sdl_rmask = 0x000000ff;
    static constexpr Uint32 sdl_gmask = 0x0000ff00;
    static constexpr Uint32 sdl_bmask = 0x00ff0000;
    static constexpr Uint32 sdl_amask = 0xff000000;
#endif

Sprite createRectangle(Vector2i dimensions, Colour const &colour)
{
    SDL_Surface* surface = SDL_CreateRGBSurface(
      0,
      dimensions.x(),
      dimensions.y(),
      32,
      sdl_rmask, sdl_gmask, sdl_bmask, sdl_amask
    );

    SDL_Rect rect = { 0, 0, dimensions.x(), dimensions.y() };

    SDL_FillRect( surface, &rect, SDL_MapRGB( surface->format, colour.raw().r, colour.raw().g, colour.raw().b ));

    SDL_Texture* texture = SDL_CreateTextureFromSurface(
      ResourceManager::get().getWindow()->renderer()->raw(),
      surface
    );

    SDL_SetTextureBlendMode( texture, SDL_BLENDMODE_BLEND );
    SDL_SetTextureAlphaMod( texture, colour.raw().a );

    if ( surface != nullptr )
    {
        SDL_FreeSurface(surface);
    }

    return Sprite{ std::make_shared<Texture>(texture) };
}

Sprite
createBorderedRectangle(Vector2i dimensions, Colour const &borderColour, Colour const &backColour, int borderWidth)
{
    SDL_Surface* surface = SDL_CreateRGBSurface(
            0,
            dimensions.x(),
            dimensions.y(),
            32,
            sdl_rmask, sdl_gmask, sdl_bmask, sdl_amask
    );

    SDL_Rect outerRect = { 0, 0, dimensions.x(), dimensions.y() };
    SDL_Rect innerRect = { borderWidth, borderWidth, dimensions.x() - 2 * borderWidth,
        dimensions.y() - 2 * borderWidth };

    SDL_FillRect( surface, &outerRect, SDL_MapRGB( surface->format,
        borderColour.raw().r, borderColour.raw().g, borderColour.raw().b ));

    SDL_FillRect( surface, &innerRect, SDL_MapRGB( surface->format,
        backColour.raw().r, backColour.raw().g, backColour.raw().b ));

    SDL_Texture* texture = SDL_CreateTextureFromSurface(
            ResourceManager::get().getWindow()->renderer()->raw(),
            surface
    );

    SDL_SetTextureBlendMode( texture, SDL_BLENDMODE_BLEND );
    SDL_SetTextureAlphaMod( texture, backColour.raw().a );

    if ( surface != nullptr )
    {
        SDL_FreeSurface(surface);
    }

    return Sprite{ std::make_shared<Texture>(texture) };
}
