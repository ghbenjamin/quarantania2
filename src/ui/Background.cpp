#include <ui/Background.h>
#include <graphics/Primatives.h>
#include <graphics/RenderInterface.h>

using namespace UI;


ElementBackground::ElementBackground(const Sprite &sprite)
    : m_type(BackgroundType::SingleSprite),
      m_singleSprite(sprite)
{

}

ElementBackground::ElementBackground(const NinePatch &patch)
    : m_type(BackgroundType::NinePatch),
      m_ninePatch(patch)
{

}

ElementBackground::ElementBackground(const Colour &colour)
    : m_type(BackgroundType::SingleColour),
      m_bgColour(colour)
{

}

ElementBackground::ElementBackground(const Colour &backColour, const Colour &borderColour, int borderWidth)
    : m_type(BackgroundType::ColourWithBorder),
      m_bgColour(backColour),
      m_borderColour(borderColour),
      m_borderWidth(borderWidth)
{

}

void ElementBackground::render(Vector2i position, RenderInterface &rInter)
{
    switch (m_type)
    {
        case BackgroundType::SingleSprite:
            m_singleSprite->renderObject( position );
            break;

        case BackgroundType::NinePatch:
            break;

        case BackgroundType::SingleColour:
        case BackgroundType::ColourWithBorder:
            rInter.addScreenItem( m_colourSprite->renderObject( position ) );
            break;
    }
}

void ElementBackground::regenerateBackground( Vector2i size )
{
    switch (m_type)
    {
        case BackgroundType::SingleSprite:
            break;
        case BackgroundType::NinePatch:
            break;
        case BackgroundType::SingleColour:
            m_colourSprite = createRectangle( size, m_bgColour );
            break;
        case BackgroundType::ColourWithBorder:
            m_colourSprite = createBorderedRectangle( size, m_borderColour, m_bgColour, m_borderWidth );
            break;
    }
}


