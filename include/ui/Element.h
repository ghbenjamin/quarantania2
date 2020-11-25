#pragma once

#include <memory>
#include <string>
#include <set>
#include <list>
#include <utils/Containers.h>
#include <ui/Defines.h>
#include <ui/Layout.h>
#include <ui/UEvent.h>
#include <resource/Sprite.h>
#include <utils/Logging.h>
#include <resource/Spritesheet.h>

class RenderInterface;
class InputInterface;

namespace UI
{

class Manager;
class Element;

using ElementPtr = std::shared_ptr<Element>;
using ElementWPtr = std::weak_ptr<Element>;
using ElementList = std::vector<ElementPtr>;

class Element : public std::enable_shared_from_this<Element>
{
    friend class Manager;

public:
    Element(Manager* manager, Element* parent);
    virtual ~Element() = default;

    // Positioning
    Vector2i globalPosition();
    Vector2i localPosition();
    void setLocalPosition(Vector2i position);
    Vector2i contentOffset() const;

    // Sizing
    Vector2i preferredContentSize() const;
    Vector2i maxOuterSize() const;

    void setPreferredContentSize( Vector2i size );
    void setPreferredOuterSize( Vector2i size );
    void setMaximumOuterSize( Vector2i size );
    bool hasMaximumOuterSize() const;
    RectI const& outerBounds() const;
    RectI const& innerBounds() const;

    // Layout
    template <typename LayoutType, typename... Args>
    void setLayout( Args... args )
    {
        m_layout = std::make_unique<LayoutType>( std::forward<Args>(args)... );
    }

    // State
    void setHidden( bool val );
    bool isHidden() const;

    // Ids
    void setId( std::string const& id );
    std::string const& id() const;

    // DOM
    void setParent( Element* elem );
    bool hasParent();
    Element* parent();
    Element* rootParent();

    template <typename T>
    std::shared_ptr<T> asType()
    {
        static_assert( std::is_base_of_v<Element, T> );
        return std::static_pointer_cast<T>( shared_from_this() );
    }

    bool hasChildren();
    std::list<ElementPtr> const& children();
    void removeChild( ElementPtr const& child );
    void removeAllChildren();

    template <typename ET>
    void addChild( std::shared_ptr<ET> const& child )
    {
        static_assert( std::is_base_of_v<Element, ET> );
        auto ptr = std::static_pointer_cast<Element>( child );

        m_children.push_back(child);

        doLayout();
    }

    // Styling
    void setBorder( int width, Colour colour );
    void removeBorder();

    void setBackgroundColour( Colour colour );
    void removeBackgroundColour();
    void setBackgroundSprite( SpritesheetKey const& sp );
    void setBackgroundSprite( Sprite const& s );
    void removeBackgroundSprite();

    void setPadding( RectI const& rect );
    void setPadding( int w );
    void setPadding( int top, int right, int bottom, int left );

    // Events

    void addEventCallback( UEventType type, UEventCallback const& callback );
    void acceptEvent(UEvent& evt);

    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);

    void doLayout();
    void holdLayout();
    void releaseLayout();

protected:

    Manager* manager();
    void onMove();
    void onSize();

private:

    template <typename Callable>
    void elementsMatchingCondition( Callable&& callable, ElementList* out )
    {
        if ( callable(shared_from_this() ))
        {
            out->push_back( shared_from_this() );
        }

        for ( auto const& c : m_children )
        {
            c->elementsMatchingCondition( callable, out );
        }
    }

    template <typename Callable>
    ElementPtr firstMatchingCondition( Callable&& callable )
    {
        ElementPtr out = ElementPtr();
        ElementPtr s_this = shared_from_this();

        if ( callable(s_this) )
        {
            out = s_this;
        }
        else
        {
            ElementPtr tmp;
            for ( auto const& c : m_children )
            {
                tmp = c->firstMatchingCondition(callable);
                if ( tmp )
                {
                    out = tmp;
                    break;
                }
            }
        }

        return out;
    }

    virtual void updateSelf(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);

    virtual void onSizeSelf();
    virtual void onMoveSelf();

    void recachePosition();
    void generateBackground();

    std::string m_id;

    Element* m_parent;
    Manager* m_manager;

    // Locations
    Vector2i m_localPosition;
    Vector2i m_globalPosition;

    // Sizes
    Vector2i m_preferredContentSize;
    Vector2i m_maxOuterSize;
    Vector2i m_contentOffset;
    RectI m_outerBounds;
    RectI m_innerBounds;

    // State
    bool m_isHidden;

    // Layout
    std::unique_ptr<UI::ElementLayout> m_layout;
    bool m_layoutHeld;

    // Styles
    bool m_hasBgColour;
    Colour m_bgColour;
    bool m_hasBorder;
    Colour m_borderColour;
    int m_borderWidth;
    RectI m_padding;

    // Events
    std::unordered_multimap<UEventType, UEventCallback> m_callbacks;

    Sprite m_backgroundSprite;
    Sprite m_backgroundTexture;
    std::list<ElementPtr> m_children;
};


}