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
#include <unordered_set>

class RenderInterface;
class InputInterface;

namespace UI
{

class Manager;
class Element;

using ElementPtr = std::shared_ptr<Element>;
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
    void setDecorative( bool val = true );
    bool isDecorative() const;

    // Ids
    void setId( std::string const& id );
    std::string const& id() const;
    void addTag( std::string const& tag );
    bool hasTag( std::string const& tag ) const;
    void removeTag( std::string const& tag );

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

protected:

    Manager* manager();
    void onMove();
    void onSize();

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

private:

    // Core
    Element* m_parent;
    Manager* m_manager;

    // Tagging
    std::string m_id;
    std::unordered_set<std::string> m_tags;

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
    bool m_isDecorative;

    // Layout
    std::unique_ptr<UI::ElementLayout> m_layout;

    // Styles
    bool m_hasBgColour;
    Colour m_bgColour;
    bool m_hasBorder;
    Colour m_borderColour;
    int m_borderWidth;
    RectI m_padding;

    // Events
    std::unordered_multimap<UEventType, UEventCallback> m_callbacks;

    // Rendering
    Sprite m_backgroundSprite;
    Sprite m_backgroundTexture;

    std::list<ElementPtr> m_children;
};


}