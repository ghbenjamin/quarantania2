#include <ui/level/TextLog.h>

#include <ui/lib/Manager.h>
#include <resource/ResourceManager.h>
#include <graphics/RenderInterface.h>

UI::TextLogLineData::TextLogLineData(const std::string &text, int count, const Colour &colour)
        : m_text(text), m_count(count), m_colour(colour) {}

void UI::TextLogLineData::setPos(Vector2i pos)
{
    m_pos = pos;
}

void UI::TextLogLineData::renderText(std::shared_ptr<FtFontFace>& font, RectI bounds)
{
    m_sprite = { font->renderString( displayText(), 14 ) };
    m_sprite.setColour( m_colour );
}

std::string UI::TextLogLineData::displayText() const
{
    if ( m_count > 1 )
    {
        return fmt::format( "{} (x{})", m_text, m_count );
    }
    else
    {
        return m_text;
    }
}

TextRenderObj const &UI::TextLogLineData::sprite() const
{
    return m_sprite;
}

RenderObject UI::TextLogLineData::getRenderObj()
{
    return m_sprite.renderObject( m_pos );
}

UI::MainTextLog::MainTextLog(Manager* manager, Element* parent)
: Element(manager, parent), m_maxLines(10), m_scrollPos(0)
{
    auto const& patch = ResourceManager::get().getNinePatch( "simple-border" );
    NinePatch np = { patch.texture(), patch.offsets() };

    setLayout<VerticalLayout>( 2, HAlignment::Left );
    setPadding(2);
    setBackground( np );
    setBorderWidth( patch.borderWidth() );
    setId("main-text-log");
    setPreferredContentSize({300, 200});

    m_font = ResourceManager::get().getDefaultFont();
}

void UI::MainTextLog::addLine(std::string_view line, Colour const& colour)
{
    // Is the content of the new line the same as the last line?
    if ( !m_lines.empty() && m_lines.back().m_text == line && m_lines.back().m_colour == colour )
    {
        // If so don't add a new line, just increment the count of our last line and rerender it
        m_lines.back().m_count++;
    }
    else
    {
        // Otherwise, push add a new line
        m_lines.push_back({ std::string{line}, 1, colour });

        // We only keep so many lines - delete from the front as we add to the back
        if ( (int) m_lines.size() > m_maxLines )
        {
            m_lines.pop_front();
        }
    }

    // We've either altered the last item or added new last item - either way
    // we need to rerender the last item & relayout
    m_lines.back().renderText(m_font, innerBounds());
    layoutLines();
}

void UI::MainTextLog::onSizeSelf()
{
    // We've changed in size - our word wrapping may have changed, so we need to rerender all of our lines
    for ( auto& line : m_lines )
    {
        line.renderText( m_font, innerBounds() );
    }

    layoutLines();
}

void UI::MainTextLog::onMoveSelf()
{
    layoutLines();
}

void UI::MainTextLog::layoutLines()
{
    int wOffset = globalPosition().x() + contentOffset().x();

    // Pixel position of the very bottom of the content area of the text box
    int hOffset = globalPosition().y() + contentOffset().y() + innerBounds().h();

    // Walk over the lines from bottom to top, putting them one after the other
    for ( auto it = m_lines.rbegin(); it != m_lines.rend(); it++ )
    {
        hOffset -= it->sprite().getSize().y();
        hOffset -= LineSpacing;

        it->setPos({wOffset, hOffset});
    }
}

void UI::MainTextLog::updateSelf(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    int currH = 0;

    for ( auto it = m_lines.rbegin(); it != m_lines.rend(); it++ )
    {
        auto rObj = it->getRenderObj();
        currH += 20; // TODO REDO THIS WHOLE THING
        currH += LineSpacing;
        rInter.addItem( rObj, RenderLayer::UI );

        // Stop drawing once we're over the top of the text box
        if ( currH >= innerBounds().h() )
        {
            break;
        }
    }
}