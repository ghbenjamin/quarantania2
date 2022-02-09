#pragma once

#include <memory>
#include <string>
#include <set>
#include <list>
#include <unordered_set>

#include <utils/Containers.h>
#include <ui/lib/Defines.h>
#include <ui/lib/Layout.h>
#include <ui/lib/UEvent.h>
#include <resource/Sprite.h>
#include <utils/Logging.h>
#include <utils/Interpolate.h>
#include <resource/Spritesheet.h>
#include <ui/lib/Background.h>


class RenderInterface;
class InputInterface;

namespace UI
{

class Manager;
class Element;
class Tooltip;

using ElementPtr = std::shared_ptr<Element>;
using ElementList = std::vector<ElementPtr>;

class Element : public std::enable_shared_from_this<Element>
{
    friend class Manager;

public:
    Element(Manager* manager, Element* parent);
    virtual ~Element() = default;

    // Positioning
    Vector2i globalPosition() const;
    Vector2i localPosition();
    void setLocalPosition(Vector2i position);
    void setLocalPosition(int x, int y);
    Vector2i contentOffset() const;

    // Sizing
    Vector2i preferredContentSize() const;
    Vector2i maxOuterSize() const;

    void setPreferredContentSize( Vector2i size );
    void setPreferredContentWidth( int width );
    void setPreferredOuterSize( Vector2i size );
    void setMaximumOuterSize( Vector2i size );
    bool hasMaximumOuterSize() const;
    RectI const& outerBounds() const;
    RectI const& innerBounds() const;

    // Layout
    template <typename LayoutType, typename... Args>
    void setLayout( Args... args )
    {
        m_layout = std::make_unique<LayoutType>( this, std::forward<Args>(args)... );
    }

    void setLayout( std::unique_ptr<ElementLayout>&& layout );


    // State
    void setHidden( bool val );
    bool isHidden() const;
    void hide();
    void show();
    void setDecorative( bool val = true );
    bool isDecorative() const;

    // Ids
    void setId( std::string const& id );
    std::string const& id() const;
    void addTag( std::string const& tag );
    bool hasTag( std::string const& tag ) const;
    void removeTag( std::string const& tag );

    // Tooltips
    bool hasTooltipSpawner() const;
    void setTooltipSpawner( std::function<std::shared_ptr<Tooltip>()> spawner );
    std::shared_ptr<Tooltip> generateTooltip();

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
    void removeChild( Element* child );
    void removeAllChildren();
    
    void deleteSelf();

    template <typename ET>
    void addChild( std::shared_ptr<ET> const& child )
    {
        static_assert( std::is_base_of_v<Element, ET> );
        auto ptr = std::static_pointer_cast<Element>( child );
        
        m_children.push_back(child);
        
        if (m_alphaMod.has_value())
        {
            m_children.back()->setAlphaMod(*m_alphaMod);
        }
        
        doLayout();
    }

    // Styling
    void setBorder( Colour background, int width, Colour colour );

    void setBorderWidth( int width );

    template <typename ... Args>
    void setBackground( Args ... args )
    {
        m_background = { std::forward<Args>(args)... };
        generateBackground();
    }

    void removeBackround();
    bool hasBackground() const;
    ElementBackground& getBackground();
    
    // Alpha mods for fades in/out
    std::optional<float> getAlphaMod();
    bool hasActiveAlphaTransition() const;
    void setAlphaMod( float value );
    void setAlphaTransition(float start, float, float time);
    void setFadeIn();

    void setBoundsScissoring(bool val);

    void setPadding( RectI const& rect );
    void setPadding( int w );
    void setPadding( int top, int right, int bottom, int left );

    // Modal behaviour
    void setIsModal( bool value );
    bool isModal() const;

    // Events

    void addEventCallback( UEventType type, UEventCallback const& callback );
    void removeEventCallbacks( UEventType type );
    void acceptEvent(UEvent& evt);
    
    void addHotkey(SDL_Keycode key, std::function<void()> const& callback);
    bool hasHotkey(SDL_Keycode key) const;
    void removeHotkey(SDL_Keycode key);
    
    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);
    void doLayout();

protected:

    Manager* manager();
    void onMove();
    void onSize();
    virtual void onAlphaModChange(float newValue);

    // Recursively walk the children of this element, adding any which match the specified condition
    // to the supplied list
    template <typename Callable>
    void descElementsMatchingCondition( Callable&& callable, ElementList* out )
    {
        if ( callable( shared_from_this() ))
        {
            out->push_back( shared_from_this() );
        }

        for ( auto const& c : m_children )
        {
            c->descElementsMatchingCondition(callable, out);
        }
    }

    // Recursively walk the children of this element, returning the first element which matches the supplied condition
    template <typename Callable>
    Element* firstDescMatchingCondition( Callable&& callable )
    {
        Element* out = nullptr;

        if ( callable(this) )
        {
            out = this;
        }
        else
        {
            Element* tmp;
            for ( auto const& c : m_children )
            {
                tmp = c->firstDescMatchingCondition(callable);
                if ( tmp )
                {
                    out = tmp;
                    break;
                }
            }
        }

        return out;
    }

    // Walk the parents of this element, returning the first element matching the specified condition,
    // if it exists.
    template <typename Callable>
    Element* closestAncestor( Callable&& callable )
    {
        Element* current = parent();
        while ( current != nullptr )
        {
            if ( callable(current) )
            {
                return current;
            }
            else
            {
                current = current->parent();
            }
        }

        return current;
    }

    Element* closestAncestorWithTag( std::string const& tag )
    {
        return closestAncestor( [&](auto const& elem){ return elem->hasTag(tag); });
    }
    
    
    template <typename T = Element>
    std::shared_ptr<T> withId( std::string const& id )
    {
        auto ptr = firstDescMatchingCondition([&](auto const& e){
            return e->id() == id;
        });
        
        return ptr->asType<T>();
    }
    
    // Behaviour hooks to implement in children
    virtual void updateSelf(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);
    virtual void onSizeSelf();
    virtual void onMoveSelf();
    
    // Regenerate our cached global position, e.g. if one of our ancestors has moved
    void recachePosition();
    
    // Regenerate our background sprite if we have one, e.g. after we've changed size
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
    bool m_isModal;

    // Layout
    std::unique_ptr<UI::ElementLayout> m_layout;

    // Styles
    std::optional<ElementBackground> m_background;
    int m_borderWidth;
    RectI m_padding;

    // Alpha modifications for fade in / out
    std::optional<float> m_alphaMod;
    std::optional<TimedLinearInterpolator<float>> m_alphaTransition;
    bool m_shouldBoundsScissor;

    // Events
    std::unordered_multimap<UEventType, UEventCallback> m_callbacks;
    std::unordered_map<SDL_Keycode, std::function<void()>> m_hotkeys;
    
    // DOM
    std::list<ElementPtr> m_children;
    
    // Tooltips
    std::optional< std::function<std::shared_ptr<Tooltip>()> > m_tooltipSpawner;
};


}