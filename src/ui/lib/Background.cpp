#include <ui/lib/Background.h>
#include <graphics/Primatives.h>
#include <graphics/RenderInterface.h>
#include <resource/ResourceManager.h>

using namespace UI;


ElementBackground::ElementBackground(const Sprite &sprite)
    : m_type(BackgroundType::SingleSprite),
      m_singleSprite(sprite)
{
    m_singleSprite->setRenderLayer(RenderLayer::UI);
}

ElementBackground::ElementBackground(const SpritesheetKey &key)
    : m_type(BackgroundType::SingleSprite)
{
    m_singleSprite = ResourceManager::get().getSprite( key );
    m_singleSprite->setRenderLayer(RenderLayer::UI);
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
            rInter.addItem( m_singleSprite->renderObject( position ), RenderLayer::UI );
            break;

        case BackgroundType::NinePatch:
            m_ninePatch->render( position, rInter );
            break;

        case BackgroundType::SingleColour:
        case BackgroundType::ColourWithBorder:
            if (m_colourSprite.has_value())
            {
                rInter.addItem( m_colourSprite->renderObject( position ), RenderLayer::UI );
            }
            break;
    }
}

void ElementBackground::regenerateBackground( Vector2i size )
{
    if ( size.x() == 0 || size.y() == 0 )
    {
        return;
    }

    switch (m_type)
    {
        case BackgroundType::SingleSprite:
            break;
        case BackgroundType::NinePatch:
            m_ninePatch->setSize( size );
            if ( m_hasColourMod )
                m_ninePatch->setColourMod( m_elementColourMod );
            break;
        case BackgroundType::SingleColour:
            m_colourSprite = createRectangle( size, m_bgColour );
            m_colourSprite->setRenderLayer( RenderLayer::UI );
            if ( m_hasColourMod )
                m_colourSprite->setColour( m_elementColourMod );
            break;
        case BackgroundType::ColourWithBorder:
            m_colourSprite = createBorderedRectangle( size, m_borderColour, m_bgColour, m_borderWidth );
            m_colourSprite->setRenderLayer( RenderLayer::UI );
            if ( m_hasColourMod )
                m_colourSprite->setColour( m_elementColourMod );
            break;
    }
}

void ElementBackground::setColourMod(Colour colour)
{
    m_elementColourMod = colour;
    m_hasColourMod = true;
    
    if (m_colourSprite.has_value())
    {
        m_colourSprite->setColour( colour );
    }
    else if (m_singleSprite.has_value())
    {
        m_singleSprite->setColour(colour);
    }
    else if (m_ninePatch.has_value())
    {
        m_ninePatch->setColourMod(colour);
    }
}

NinePatch::NinePatch(TexturePtr texture, const std::array<RectI, 9> &offsets)
{
    for (int i = 0; i < 9; i++)
    {
        m_sprites[i] = Sprite( texture, offsets[i] );
        m_sprites[i].setRenderLayer( RenderLayer::UI );
        m_offsets[i] = {0, 0, 0, 0};
    }
}

void NinePatch::setSize(Vector2i size)
{
    int x0 = 0;
    int x1 = m_sprites[(int)Nontants::TopLeft].size().x();
    int x2 = size.x() - m_sprites[(int)Nontants::TopRight].size().x();

    int y0 = 0;
    int y1 = m_sprites[(int)Nontants::TopLeft].size().y();
    int y2 = size.y() - m_sprites[(int)Nontants::BottomLeft].size().y();

    int leftColW = x1;
    int midColW = x2 - x1;
    int rightColW =  m_sprites[(int)Nontants::TopRight].size().x();

    int topRowH = y1;
    int midRowH = y2 - y1;
    int bottomRowH = m_sprites[(int)Nontants::BottomLeft].size().y();

    m_offsets[(int)Nontants::TopLeft] =      { x0, y0, leftColW, topRowH };
    m_offsets[(int)Nontants::TopCentre] =    { x1, y0, midColW, topRowH };
    m_offsets[(int)Nontants::TopRight] =     { x2, y0, rightColW, topRowH };

    m_offsets[(int)Nontants::CentreLeft] =   { x0, y1, leftColW, midRowH };
    m_offsets[(int)Nontants::Centre] =       { x1, y1, midColW, midRowH };
    m_offsets[(int)Nontants::CentreRight] =  { x2, y1, rightColW, midRowH };

    m_offsets[(int)Nontants::BottomLeft] =   { x0, y2, leftColW, bottomRowH };
    m_offsets[(int)Nontants::BottomCentre] = { x1, y2, midColW, bottomRowH };
    m_offsets[(int)Nontants::BottomRight] =  { x2, y2, rightColW, bottomRowH };

    m_sprites[(int)Nontants::TopCentre].setTargetSize({ midColW, topRowH });
    m_sprites[(int)Nontants::CentreLeft].setTargetSize({ leftColW, midRowH });
    m_sprites[(int)Nontants::CentreRight].setTargetSize({ rightColW, midRowH });
    m_sprites[(int)Nontants::BottomCentre].setTargetSize({ midColW, bottomRowH });
    m_sprites[(int)Nontants::Centre].setTargetSize({ midColW, midRowH });
}

void NinePatch::render(Vector2i position, RenderInterface &rInter)
{
    for (int i = 0; i < 9; i++)
    {
        rInter.addItem( m_sprites[i].renderObject( position + m_offsets[i].left() ), RenderLayer::UI );
    }
}

void NinePatch::setColourMod( Colour colour )
{
    for (int i = 0; i < 9; i++)
    {
        m_sprites[i].setColour( colour );
    }
}