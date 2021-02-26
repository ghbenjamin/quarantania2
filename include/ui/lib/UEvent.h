#pragma once

#include <memory>
#include <functional>

#include <utils/Containers.h>

namespace UI
{

enum class UEventType
{
    MouseDown,
    MouseUp,
    Click,

    MouseIn,
    MouseOut,
    MouseMove,

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

class Element;
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
    };
};

using UEventCallback = std::function<void(UEvent&)>;

}