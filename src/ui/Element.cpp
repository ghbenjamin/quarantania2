#include <ui/Element.h>

#include <algorithm>
#include <utils/Logging.h>
#include <graphics/Primatives.h>
#include <graphics/RenderInterface.h>
#include <utils/Assert.h>
#include <resource/ResourceManager.h>

using namespace UI;

Element::Element() :
    m_parent(nullptr), m_manager(nullptr),
    m_hasBgColour(false), m_hasBorder(false),
    m_borderWidth(0), m_isHidden(false), m_layoutHeld(false),
    m_maxOuterSize({0, 0})
{
    // Sensible default
    setLayout<UI::VerticalLayout>();
}

void Element::setId(std::string const &id)
{
    m_id = id;
}

void Element::unsetId()
{
    m_id = "";
}

std::string const& Element::id() const
{
    return m_id;
}

bool Element::hasId() const
{
    return m_id.empty();
}

void Element::addClass(std::string const &c)
{
    m_classes.insert(c);
}

void Element::removeClass(std::string const &c)
{
    m_classes.erase(c);
}

bool Element::hasClass(std::string const &c) const
{
    return m_classes.find(c) != m_classes.end();
}

void Element::setParent(Element* elem)
{
    m_parent = elem;
}

void Element::updateSelf(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter)
{

}

Element *Element::parent()
{
    return m_parent;
}

bool Element::hasParent()
{
    return m_parent != nullptr;
}

void Element::update(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    if ( m_backgroundSprite )
    {
        rInter.addScreenItem( m_backgroundSprite.renderObject( globalPosition() ) );
    }


    updateSelf(ticks, iinter, rInter);

    for ( auto& c : m_children )
    {
        c->update(ticks, iinter, rInter);
    }
}

Vector2i Element::globalPosition()
{
    return m_globalPosition;
}

Vector2i Element::localPosition()
{
    return m_localPosition;
}

void Element::setLocalPosition(Vector2i position)
{
    m_localPosition = position;
    onMove();
}

void Element::onMove()
{
    recachePosition();
    onMoveSelf();

    for ( auto const& c : m_children )
    {
        c->onMove();
    }
}

// Implement in children
void Element::onMoveSelf() { }

void Element::onSize()
{
    doLayout();

    onSizeSelf();
}

// Implement in children
void Element::onSizeSelf() { }

void Element::doLayout()
{
    // Padding: x=top, y=right, w=bottom, h=left

    m_contentOffset = {
        m_borderWidth + m_padding.h(),
        m_borderWidth + m_padding.x(),
    };

    if ( hasChildren() )
    {
        Assert( !!m_layout );
        m_actualContentSize = m_layout->doLayout(this);
    }

    // No children = size to our preferred size
    else
    {
        m_actualContentSize = m_preferredContentSize;
    }

    m_actualOuterSize = {
        (2 * m_borderWidth) + m_padding.y() + m_padding.h() + m_actualContentSize.x(),
        (2 * m_borderWidth) + m_padding.x() + m_padding.w() + m_actualContentSize.y(),
    };

    m_outerBounds = RectI{ m_globalPosition, m_actualOuterSize };
    m_innerBounds = RectI{ m_globalPosition + m_contentOffset, m_actualContentSize };

    generateBackground();
}

void Element::recachePosition()
{
    if ( hasParent() )
    {
        m_globalPosition = m_localPosition + parent()->globalPosition() + parent()->m_contentOffset;
    }
    else
    {
        m_globalPosition = m_localPosition;
    }

    m_outerBounds = RectI{ m_globalPosition, m_actualOuterSize };
    m_innerBounds = RectI{ m_globalPosition + m_contentOffset, m_actualContentSize };
}

bool Element::hasChildren()
{
    return !m_children.empty();
}

std::list<ElementPtr> const &Element::children()
{
    return m_children;
}

void Element::removeChild(ElementPtr const &child)
{
    m_children.erase( std::remove(m_children.begin(), m_children.end(), child), m_children.end() );
    child->setParent( nullptr );
}

Element *Element::rootParent()
{
    Element* curr = this;
    while ( curr->m_parent != nullptr )
    {
        curr = curr->m_parent;
    }

    return curr;
}

Vector2i Element::outerSize() const
{
    return m_actualOuterSize;
}

Vector2i Element::contentSize() const
{
    return m_actualContentSize;
}

void Element::setPreferredContentSize(Vector2i size)
{
    m_preferredContentSize = size;

    rootParent()->doLayout();
    onSizeSelf();
}

void Element::setPreferredOuterSize(Vector2i size)
{
    Vector2i preferred {
        size.x() - (2 * m_borderWidth) + m_padding.y() + m_padding.h(),
        size.y() - (2 * m_borderWidth) + m_padding.x() + m_padding.w(),
    };
    
    if ( m_preferredContentSize != preferred )
    {
        setPreferredContentSize(preferred);
    }
}

Manager *Element::manager()
{
    return m_manager;
}

void Element::setBorder(int width, Colour colour)
{
    m_hasBorder = true;
    m_borderWidth = width;
    m_borderColour = colour;

    doLayout();
}

void Element::removeBorder()
{
    m_hasBorder = false;
    m_borderWidth = 0;

    generateBackground();
}

void Element::setBackgroundColour(Colour colour)
{
    m_hasBgColour = true;
    m_bgColour = colour;

    generateBackground();
}

void Element::removeBackgroundColour()
{
    m_hasBgColour = false;

    generateBackground();
}

void Element::setPadding( RectI const& rect )
{
    m_padding = rect;

    doLayout();
}

void Element::setPadding(int w)
{
    m_padding = { w, w, w, w };

    doLayout();
}

void Element::setPadding(int top, int right, int bottom, int left)
{
    m_padding = { top, right, bottom, left };

    doLayout();
}


void Element::generateBackground()
{
    if ( m_actualOuterSize.x() == 0 || m_actualOuterSize.y() == 0 )
    {
        return;
    }

    if ( m_backgroundTexture )
    {
        m_backgroundSprite = m_backgroundTexture;
    }

    else if ( m_hasBgColour )
    {
        if ( m_hasBorder )
        {
            m_backgroundSprite = createBorderedRectangle( m_actualOuterSize, m_borderColour, m_bgColour, m_borderWidth );
        }
        else
        {
            m_backgroundSprite = createRectangle( m_actualOuterSize, m_bgColour );
        }
    }

    else
    {
        m_backgroundSprite = Sprite();
    }

    m_backgroundSprite.setRenderLayer(RenderLayer::UI );
}

void Element::setHidden( bool val )
{
    m_isHidden = val;

    rootParent()->doLayout();
}

bool Element::isHidden() const
{
    return m_isHidden;
}


Vector2i Element::preferredSize() const
{
    return m_preferredContentSize;
}

void Element::holdLayout()
{
    m_layoutHeld = true;
}

void Element::releaseLayout()
{
    m_layoutHeld = false;
    doLayout();
}

RectI const &Element::outerBounds() const
{
    return m_outerBounds;
}

RectI const &Element::innerBounds() const
{
    return m_innerBounds;
}

void Element::acceptEvent(UEvent &evt)
{
    auto range = m_callbacks.equal_range(evt.type);

    for ( auto it = range.first; it != range.second; it++ )
    {
        it->second(evt);
    }
}

void Element::addEventCallback(UEventType type, UEventCallback const &callback)
{
    m_callbacks.emplace( type, callback );
}

void Element::setBackgroundSprite(SpritesheetKey const &sp)
{
    m_backgroundTexture = ResourceManager::get().getSprite( sp );
    m_backgroundTexture.setRenderLayer(RenderLayer::UI);
    generateBackground();
}

void Element::removeBackgroundSprite()
{
    m_backgroundTexture = Sprite();
    generateBackground();
}

bool Element::hasMaximumOuterSize() const
{
    return m_maxOuterSize != Vector2i{0, 0};
}

void Element::setMaximumOuterSize(Vector2i size)
{
    m_maxOuterSize = size;
    onSize();
}

Vector2i Element::maxOuterSize() const
{
    return m_maxOuterSize;
}

Vector2i Element::contentOffset() const
{
    return m_contentOffset;
}

