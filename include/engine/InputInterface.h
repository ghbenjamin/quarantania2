#pragma once

#include <unordered_set>
#include <deque>
#include <SDL.h>
#include <utils/Containers.h>


enum class IEventType
{
    KeyPress,
    MouseDown,
    MouseUp,
    MouseMove,
    ScrollWheel,

    WindowResize
};

struct IEventKeyPress
{
    IEventKeyPress();
    explicit IEventKeyPress(SDL_Keycode key);

    SDL_Keycode keyCode;
};

struct IEventMouseDown
{
    IEventMouseDown();
    IEventMouseDown(Vector2i sp, int btn);

    Vector2i screenPos;
    int button;
};

struct IEventMouseUp
{
    IEventMouseUp();
    IEventMouseUp(Vector2i sp, int btn);

    Vector2i screenPos;
    int button;
};

struct IEventScrollWheel
{
    IEventScrollWheel();
    IEventScrollWheel( Vector2i screenPos, int magnitude);

    Vector2i screenPos;
    int magnitude;
};

struct IEventMouseMove
{
    IEventMouseMove();
    explicit IEventMouseMove(Vector2i sp);

    Vector2i screenPos;
};

struct IEventWindowResize
{
    IEventWindowResize();
    explicit IEventWindowResize(Vector2i size);

    Vector2i screenSize;
};



struct IEvent
{
    IEvent();
    ~IEvent() = default;

    IEventType type;
    union
    {
        IEventKeyPress keyPress;
        IEventMouseDown mouseDown;
        IEventMouseUp mouseUp;
        IEventMouseMove mouseMove;
        IEventScrollWheel scrollWheel;

        IEventWindowResize windowResize;
    };
};



class InputInterface
{

public:
    InputInterface();
    ~InputInterface() = default;

    void input(SDL_Event& sdlEvent);

    std::vector<IEvent>& queue();
    void clear();

    bool isHeld(SDL_Keycode keycode) const;
    bool anyHeld() const;

private:

    bool m_anyHeld;
    std::vector<IEvent> m_queue;
    std::unordered_set<SDL_Keycode> m_heldKeys;

};
