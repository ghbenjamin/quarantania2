#include <graphics/Primatives.h>
#include <resource/ResourceManager.h>
#include <utils/Math.h>

Sprite createRectangle(Vector2i dimensions, Colour const &colour)
{
    return Sprite{ createRectangleRObj(dimensions, colour), dimensions };
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

RenderObject createRectangleRObj(Vector2i dimensions, Colour colour)
{
    auto [r, g, b, a] = colour.asOpenGL();

    RenderObject obj{0, ShaderType::ColourShader};
    obj.setScreenVerts(0, 0, 0, (float)dimensions.x(), (float)dimensions.y());
    obj.setColourVerts( 0, r, g, b, a );

    return obj;
}