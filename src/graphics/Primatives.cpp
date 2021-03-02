#include <graphics/Primatives.h>
#include <resource/ResourceManager.h>

Sprite createRectangle(Vector2i dimensions, Colour const &colour)
{
    auto surface = Surface::createSurface( dimensions );

    SDL_Rect rect = { 0, 0, dimensions.x(), dimensions.y() };

    SDL_FillRect(
        surface->raw(),
        &rect,
        SDL_MapRGB( surface->raw()->format, colour.raw().r, colour.raw().g, colour.raw().b )
    );

    SDL_Texture* texture = SDL_CreateTextureFromSurface(
      ResourceManager::get().getWindow()->renderer(),
      surface->raw()
    );

    SDL_SetTextureBlendMode( texture, SDL_BLENDMODE_BLEND );
    SDL_SetTextureAlphaMod( texture, colour.raw().a );

    return Sprite{ std::make_shared<Texture>(texture) };
}

Sprite
createBorderedRectangle(Vector2i dimensions, Colour const &borderColour, Colour const &backColour, int borderWidth)
{
    auto surface = Surface::createSurface( dimensions );

    SDL_Rect outerRect = { 0, 0, dimensions.x(), dimensions.y() };
    SDL_Rect innerRect = { borderWidth, borderWidth, dimensions.x() - 2 * borderWidth,
        dimensions.y() - 2 * borderWidth };

    SDL_FillRect( surface->raw(), &outerRect, SDL_MapRGB( surface->raw()->format,
        borderColour.raw().r, borderColour.raw().g, borderColour.raw().b ));

    SDL_FillRect( surface->raw(), &innerRect, SDL_MapRGB( surface->raw()->format,
        backColour.raw().r, backColour.raw().g, backColour.raw().b ));

    SDL_Texture* texture = SDL_CreateTextureFromSurface(
            ResourceManager::get().getWindow()->renderer(),
            surface->raw()
    );

    SDL_SetTextureBlendMode( texture, SDL_BLENDMODE_BLEND );
    SDL_SetTextureAlphaMod( texture, backColour.raw().a );

    return Sprite{ std::make_shared<Texture>(texture) };
}
