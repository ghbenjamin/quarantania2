#include <ui/lib/Element.h>
#include <ui/lib/Manager.h>
#include <algorithm>
#include <utils/Logging.h>
#include <graphics/Primatives.h>
#include <graphics/RenderInterface.h>
#include <utils/Assert.h>
#include <resource/ResourceManager.h>

using namespace UI;

Element::Element(Manager* manager, Element* parent)
  : m_parent(parent),
    m_manager(manager),
    m_borderWidth(0),
    m_isHidden(false),
    m_maxOuterSize({0, 0}),
    m_isDecorative(false)
{
    // Sensible default
    setLayout<UI::VerticalLayout>();
}

void Element::setId(std::string const &id)
{
    m_id = id;
}

std::string const& Element::id() const
{
    return m_id;
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
    // If we're hidden, pretend we don't exist
    if ( isHidden() )
    {
        return;
    }

    // Render our background, if any
    if ( m_background.has_value() )
    {
        m_background->render( globalPosition(), rInter );
    }

    // Update and render ourself
    updateSelf(ticks, iinter, rInter);

    // Update and render our children, if any
    for ( auto& c : m_children )
    {
        c->update(ticks, iinter, rInter);
    }
}

Vector2i Element::globalPosition() const
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

    // If we're hidden, we have no area
    if ( isHidden() )
    {
        m_contentOffset = {0, 0};
        m_outerBounds = { m_globalPosition, {0, 0} };
        m_innerBounds = { m_globalPosition, {0, 0} };
        return;
    }

    Vector2i actualContentSize;

    m_contentOffset = {
        m_borderWidth + m_padding.h(),
        m_borderWidth + m_padding.x(),
    };

    // We have children - size to the size of our children, w.r.t. our current layout
    if ( hasChildren() )
    {
        Assert( !!m_layout );
        actualContentSize = m_layout->doLayout(this);
    }

    // No children = size to our preferred size
    else
    {
        actualContentSize = m_preferredContentSize;
    }

    if ( m_id == "ui-equip-inner")
    {
        int t = 4;
    }

    m_outerBounds = RectI{
        m_globalPosition.x(),
        m_globalPosition.y(),
        (2 * m_borderWidth) + m_padding.y() + m_padding.h() + actualContentSize.x(),
        (2 * m_borderWidth) + m_padding.x() + m_padding.w() + actualContentSize.y(),
    };

    m_innerBounds = RectI{ m_globalPosition + m_contentOffset, actualContentSize };

    generateBackground();

    // If we're a root level node, we might need to be moved by the manager relative to the window
    if (m_parent == nullptr)
    {
        manager()->doLayout();
    }
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

    m_outerBounds = RectI{ m_globalPosition, m_outerBounds.right() };
    m_innerBounds = RectI{ m_globalPosition + m_contentOffset, m_innerBounds.right() };
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

void Element::removeAllChildren()
{
    for ( auto const& child : m_children )
    {
        child->setParent(nullptr);
    }

    m_children.clear();
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

void Element::setPreferredContentSize(Vector2i size)
{
    m_preferredContentSize = size;

    if ( m_parent != nullptr )
    {
        m_parent->doLayout();
    }
    else
    {
        doLayout();
    }

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

void Element::setBorder(Colour background, int width, Colour colour)
{
    setBackground( background, colour, width );

    m_borderWidth = width;
    doLayout();
}


void Element::setBorderWidth(int width)
{
    m_borderWidth = width;
    doLayout();
}


void Element::setPadding( RectI const& rect )
{
    m_padding = rect;
    doLayout();
}

void Element::setPadding(int w)
{
    setPadding(w, w, w, w);
}

void Element::setPadding(int top, int right, int bottom, int left)
{
    m_padding = { top, right, bottom, left };
    doLayout();
}


void Element::generateBackground()
{

    if ( m_background.has_value() )
    {
        m_background->regenerateBackground( m_outerBounds.right() );
    }
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


Vector2i Element::preferredContentSize() const
{
    return m_preferredContentSize;
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

void Element::setDecorative(bool val)
{
    m_isDecorative = val;
}

bool Element::isDecorative() const
{
    return m_isDecorative;
}

void Element::addTag(const std::string &tag)
{
    m_tags.insert(tag);
}

bool Element::hasTag(const std::string &tag) const
{
    return m_tags.find(tag) != m_tags.end();
}

void Element::removeTag(const std::string &tag)
{
    m_tags.erase(tag);
}

void Element::removeBackround()
{
    m_background.reset();
}

void Element::hide()
{
    setHidden(true);
}

void Element::show()
{
    setHidden(false);
}


