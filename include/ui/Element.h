#pragma once

#include <memory>
#include <string>
#include <set>
#include <utils/Containers.h>
#include <ui/Defines.h>
#include <ui/Layout.h>
#include <resource/Sprite.h>

class RenderInterface;
class InputInterface;

namespace UI
{

class UiManager;
class Element;

using ElementPtr = std::shared_ptr<Element>;
using ElementWPtr = std::weak_ptr<Element>;
using ElementList = std::vector<ElementPtr>;

class Element
{
    friend class UiManager;

public:
    Element();
    virtual ~Element() = default;

    // Positioning
    Vector2i globalPosition();
    Vector2i localPosition();
    void setLocalPosition(Vector2i position);

    // Sizing
    Vector2i outerSize() const;
    Vector2i contentSize() const;
    Vector2i preferredSize() const;
    void setPreferredContentSize( Vector2i size );
    void setPreferredOuterSize( Vector2i size );
    void setMaximumOuterSize( Vector2i size );

    RectI const& bounds() const;

    // Layout
    template <typename LayoutType, typename... Args>
    void setLayout( Args... args )
    {
        m_layout = std::make_unique<LayoutType>( std::forward(args)... );
    }

    // State
    void setHidden( bool val );
    bool isHidden() const;

    // Ids
    void setId( std::string const& id );
    void unsetId();
    std::string const& id() const;
    bool hasId() const;

    // Classes
    void addClass( std::string const& c );
    void removeClass( std::string const& c );
    bool hasClass( std::string const& c );

    // DOM
    void setParent( Element* elem );
    bool hasParent();
    Element* parent();
    Element* rootParent();

    bool hasChildren();
    std::vector<ElementPtr> const& children();
    void removeChild( ElementPtr const& child );

    template <typename ET>
    void addChild( std::shared_ptr<ET> const& child )
    {
        static_assert( std::is_base_of_v<Element, ET> );
        auto ptr = std::static_pointer_cast<Element>( child );

        m_children.push_back(child);
        child->setParent( this );

        doLayout();
    }

    ElementPtr descWithId( std::string const& id ) const;

    // Styling
    void setBorder( int width, Colour colour );
    void removeBorder();
    void setBackgroundColour( Colour colour );
    void removeBackgroundColour();

    void setPadding( RectI const& rect );
    void setPadding( int w );
    void setPadding( int top, int right, int bottom, int left );

    UiManager* manager();
    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);

    void doLayout();

    void holdLayout();
    void releaseLayout();

protected:

    void onMove();
    void onSize();

private:

    virtual void updateSelf(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);

    virtual void onSizeSelf();
    virtual void onMoveSelf();

    void recachePosition();

    void generateBackground();

    std::string m_id;
    std::set<std::string> m_classes;

    Element* m_parent;
    UiManager* m_manager;

    // Locations
    Vector2i m_localPosition;
    Vector2i m_globalPosition;

    // Sizes
    Vector2i m_actualContentSize;
    Vector2i m_actualOuterSize;
    Vector2i m_maximumOuterSize;
    Vector2i m_preferredContentSize;
    Vector2i m_contentOffset;
    RectI m_bounds;

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

    Sprite m_backgroundSprite;

    std::vector<ElementPtr> m_children;
};


}