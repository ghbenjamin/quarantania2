#pragma once

#include <memory>
#include <string>
#include <set>
#include <utils/Containers.h>
#include <ui/Defines.h>

class RenderInterface;
class InputInterface;

namespace UI
{

class Element;

using ElementPtr = std::shared_ptr<Element>;
using ElementWPtr = std::weak_ptr<Element>;

class Element
{
public:
    Element();
    virtual ~Element() = default;

    Vector2i globalPosition();
    Vector2i localPosition();
    void setLocalPosition(Vector2i position);

    Vector2i outerSize();
    Vector2i contentSize();
    void setPreferredContentSize( Vector2i size );
    void setMaximumOuterSize( Vector2i size );

    void setId( std::string const& id );
    void unsetId();
    std::string_view id();
    bool hasId();

    void addClass( std::string const& c );
    void removeClass( std::string const& c );
    bool hasClass( std::string const& c );

    void setStyle(ElementStyle const& style);
    ElementStyle& getStyle();

    void setParent( Element* elem );
    bool hasParent();
    Element* parent();
    Element* rootParent();

    bool hasChildren();
    std::vector<ElementPtr> const& children();
    void addChild( ElementPtr const& child );
    void removeChild( ElementPtr const& child );

    void update(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);

protected:

    void onMove();
    void onSize();
    void doLayout();

private:

    virtual void updateSelf(uint32_t ticks, InputInterface& iinter, RenderInterface &rInter);

    virtual void onSizeSelf();
    virtual void onMoveSelf();

    void recachePosition();

    std::string m_id;
    std::set<std::string> m_classes;

    Element* m_parent;

    Vector2i m_localPosition;
    Vector2i m_globalPosition;

    Vector2i m_actualContentSize;
    Vector2i m_actualOuterSize;
    Vector2i m_maximumOuterSize;
    Vector2i m_preferredContentSize;
    Vector2i m_contentOffset;

    ElementStyle m_style;

    std::vector<ElementPtr> m_children;
};


}