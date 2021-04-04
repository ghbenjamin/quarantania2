#pragma once

#include <array>
#include <optional>

#include <resource/Sprite.h>
#include <utils/Colour.h>

class RenderInterface;
class InputInterface;
struct SpritesheetKey;

namespace UI
{


// The portions of a rectangle cut into thirds vertically and horizontally
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



class NinePatch
{
public:

    NinePatch( TexturePtr texture, std::array<RectI, 9> const& offsets, int borderWidth );
    ~NinePatch() = default;

    int getBorderWidth() const;

    void setSize( Vector2i size );
    void render( Vector2i position, RenderInterface &rInter );
    void setColourMod( Colour colour );
    void setAlphaMod( float alpha );

private:

    int m_borderWidth;
    std::array<Sprite, 9> m_sprites;
    std::array<RectI, 9> m_offsets;
};


class ElementBackground
{
public:
    ElementBackground( Sprite const& sprite );
    ElementBackground( SpritesheetKey const& key );
    ElementBackground( NinePatch const& patch );
    ElementBackground( Colour const& colour );
    ElementBackground( Colour const& backColour, Colour const& borderColour, int borderWidth );

    ~ElementBackground() = default;

    void render(Vector2i position, RenderInterface &rInter);
    void regenerateBackground( Vector2i size );
    void setColourMod( Colour colour );
    void setAlphaMod( float alpha );

private:

    enum class BackgroundType
    {
        SingleSprite,
        NinePatch,
        SingleColour,
        ColourWithBorder,
    };

    BackgroundType m_type;

    std::optional<Sprite> m_singleSprite;
    std::optional<NinePatch> m_ninePatch;
    std::optional<Sprite> m_colourSprite;

    Colour m_bgColour;
    Colour m_borderColour;
    int m_borderWidth = 0;
    
    // Our element might have a colour modification applied. We need to apply that colour to our background
    // sprites as well.
    bool m_hasColourMod = false;
    Colour m_elementColourMod;
    std::optional<float> m_elementAlphaMod;
};


}
