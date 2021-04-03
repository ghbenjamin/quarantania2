#pragma once

#include <memory>
#include <functional>

#include <utils/Containers.h>

namespace UI
{

// Forward definitions
class Element;

enum class UEventType
{
    MouseDown,
    MouseUp,
    Click,

    MouseIn,
    MouseOut,
    MouseMove,

    DragStart,
    DragMove,
    DragStop,

    KeyDown,
    KeyUp,
    KeyPress,
};

struct UMouseButtonEvent
{
    int button;
    Vector2i pos;

    static constexpr int LeftMouseButton = 1;
    static constexpr int RightMouseButton = 3;
};

struct UMouseMoveEvent
{
    Vector2i pos;
};

struct UKeyEvent
{

};

struct UDragEvent
{
    Vector2i startPos;
    Vector2i currPos;
    Element* originalTarget;
};


struct UEvent
{
    UEvent() : type(UEventType::MouseDown) {}
    ~UEvent() = default;

    UEventType type;
    std::shared_ptr<Element> targetElement;

    bool stopPropagation = false;

    union
    {
        UMouseButtonEvent mouseButtonEvent;
        UMouseMoveEvent mouseMoveEvent;
        UKeyEvent keyEvent;
        UDragEvent dragEvent;
    };
};

using UEventCallback = std::function<void(UEvent&)>;

}