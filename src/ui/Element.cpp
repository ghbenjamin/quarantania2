#include <ui/Element.h>

#include <algorithm>
#include <utils/Logging.h>
#include <graphics/Primatives.h>
#include <graphics/RenderInterface.h>

using namespace UI;

Element::Element() :
    m_parent(nullptr), m_manager(nullptr),
    m_hasBgColour(false), m_hasBorder(false),
    m_borderWidth(0)
{

}

void Element::setId(std::string const &id)
{
    m_id = id;
}

void Element::unsetId()
{
    m_id = "";
}

std::string_view Element::id()
{
    return m_id;
}

bool Element::hasId()
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

bool Element::hasClass(std::string const &c)
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
    if ( m_hasBgColour )
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

void Element::onMoveSelf() { }

void Element::onSize()
{
    onSizeSelf();
}

void Element::onSizeSelf()
{

}

void Element::recachePosition()
{
    if ( hasParent() )
    {
        m_globalPosition = m_localPosition + parent()->globalPosition();
    }
    else
    {
        m_globalPosition = m_localPosition;
    }
}

bool Element::hasChildren()
{
    return !m_children.empty();
}

std::vector<ElementPtr> const &Element::children()
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

Vector2i Element::outerSize()
{
    return m_actualOuterSize;
}

Vector2i Element::contentSize()
{
    return m_actualContentSize;
}

void Element::setPreferredContentSize(Vector2i size)
{
    m_preferredContentSize = size;
}

void Element::setMaximumOuterSize(Vector2i size)
{
    m_maximumOuterSize = size;
}

UiManager *Element::manager()
{
    return m_manager;
}

void Element::setBorder(int width, Colour colour)
{
    m_hasBorder = true;
    m_borderWidth = width;
    m_borderColour = colour;

    generateBackground();
}

void Element::removeBorder()
{
    m_hasBorder = false;
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
}

void Element::setPadding( RectI const& rect )
{
    m_padding = rect;
}

void Element::generateBackground()
{
    if ( m_hasBgColour )
    {
        if ( m_hasBorder )
        {
            m_backgroundSprite = createBorderedRectangle( m_actualOuterSize, m_borderColour, m_bgColour, m_borderWidth );
        }
        else
        {
            m_backgroundSprite = createRectangle( m_actualOuterSize ,m_bgColour );
        }
    }
}
