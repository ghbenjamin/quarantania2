#include <graphics/Primatives.h>
#include <resource/ResourceManager.h>

Sprite createRectangle(Vector2i dimensions, Colour const &colour)
{
    auto surface = std::make_shared<Surface>( dimensions );

    SDL_Rect rect = { 0, 0, dimensions.x(), dimensions.y() };

    SDL_FillRect( surface->raw(), &rect,
        SDL_MapRGBA( surface->raw()->format, colour.raw().r, colour.raw().g, colour.raw().b, colour.raw().a )
    );

    return Sprite{ surface->toTexture() };
}

Sprite
createBorderedRectangle(Vector2i dimensions, Colour const &borderColour, Colour const &backColour, int borderWidth)
{
    auto surface = std::make_shared<Surface>( dimensions );

    SDL_Rect outerRect = { 0, 0, dimensions.x(), dimensions.y() };
    SDL_Rect innerRect = {
        borderWidth, borderWidth, dimensions.x() - 2 * borderWidth,
        dimensions.y() - 2 * borderWidth
    };

    SDL_FillRect( surface->raw(), &outerRect, SDL_MapRGBA( surface->raw()->format,
        borderColour.raw().r, borderColour.raw().g, borderColour.raw().b, borderColour.raw().a ));

    SDL_FillRect( surface->raw(), &innerRect, SDL_MapRGBA( surface->raw()->format,
        backColour.raw().r, backColour.raw().g, backColour.raw().b, backColour.raw().a ));

    return Sprite{ surface->toTexture() };
}
