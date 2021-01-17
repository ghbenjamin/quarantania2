#pragma once

#include <array>
#include <optional>

#include <resource/Sprite.h>
#include <utils/Colour.h>

class RenderInterface;
class InputInterface;

namespace UI
{

enum class Nontants
{
    TopLeft,
    TopCentre,
    TopRight,
    CentreLeft,
    Centre,
    CentreRight,
    BottomLeft,
    BottomCentre,
    BottomRight
};

enum class BackgroundType
{
    SingleSprite,
    NinePatch,
    SingleColour,
    ColourWithBorder,
};


class NinePatch
{
public:

    void setSize( Vector2i size );
    void render( Vector2i position, RenderInterface &rInter );

private:
    std::array<Sprite, 9> m_textures;
    std::array<Vector2i, 9> m_offsets;
};


class ElementBackground
{
public:
    ElementBackground( Sprite const& sprite );
    ElementBackground( NinePatch const& );
    ElementBackground( Colour const& colour );
    ElementBackground( Colour const& backColour, Colour const& borderColour, int borderWidth );

    ~ElementBackground() = default;


    void render(Vector2i position, RenderInterface &rInter);
    void regenerateBackground( Vector2i size );

private:

    const BackgroundType m_type;

    std::optional<Sprite> m_singleSprite;
    std::optional<NinePatch> m_ninePatch;
    std::optional<Sprite> m_colourSprite;

    Colour m_bgColour;
    Colour m_borderColour;
    int m_borderWidth = 0;
};


}
