#include <graphics/Primatives.h>
#include <resource/ResourceManager.h>

Sprite createRectangle(Vector2i dimensions, Colour const &colour)
{
    auto surface = std::make_shared<Surface>( dimensions );
    auto colourSDL = colour.asSDL();

    SDL_Rect rect = { 0, 0, dimensions.x(), dimensions.y() };

    SDL_FillRect( surface->raw(), &rect,
        SDL_MapRGBA( surface->raw()->format, colourSDL.r, colourSDL.g, colourSDL.b, colourSDL.a )
    );

    return Sprite{ surface->toTexture() };
}

Sprite
createBorderedRectangle(Vector2i dimensions, Colour const &borderColour, Colour const &backColour, int borderWidth)
{
    auto surface = std::make_shared<Surface>( dimensions );
    auto borderColourSDL = borderColour.asSDL();
    auto backColourSDL = backColour.asSDL();

    SDL_Rect outerRect = { 0, 0, dimensions.x(), dimensions.y() };
    SDL_Rect innerRect = {
        borderWidth, borderWidth, dimensions.x() - 2 * borderWidth,
        dimensions.y() - 2 * borderWidth
    };

    SDL_FillRect( surface->raw(), &outerRect, SDL_MapRGBA( surface->raw()->format,
       borderColourSDL.r, borderColourSDL.g, borderColourSDL.b, borderColourSDL.a ));

    SDL_FillRect( surface->raw(), &innerRect, SDL_MapRGBA( surface->raw()->format,
       backColourSDL.r, backColourSDL.g, backColourSDL.b, backColourSDL.a ));

    return Sprite{ surface->toTexture() };
}
