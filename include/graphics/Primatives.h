#pragma once

#include <resource/Sprite.h>
#include <utils/Colour.h>


RenderObject createRectangleRObj( Vector2i dimensions, Colour colour );

Sprite createRectangle( Vector2i dimensions, Colour const& colour );

Sprite createBorderedRectangle( Vector2i dimensions, Colour const& borderColour,
    Colour const& backColour, int borderWidth );