#pragma once

#include <unordered_set>
#include <deque>
#include <SDL.h>

#include <utils/Containers.h>


enum class IEventType
{
    KeyPress,
    MouseClick,
    MouseMove,
};

struct IEventKeyPress
{
    IEventKeyPress();
    IEventKeyPress(SDL_Keycode key);

    SDL_Keycode keyCode;
};

struct IEventClick
{
    IEventClick();
    IEventClick(Vector2i sp, Vector2i wp, int btn);

    Vector2i screenPos;
    Vector2i worldPos;

    int button;
};

struct IEventMouseMove
{
    IEventMouseMove();
    IEventMouseMove(Vector2i sp, Vector2i wp);

    Vector2i screenPos;
    Vector2i worldPos;
};

struct IEvent
{
    IEvent();
    ~IEvent() = default;

    IEventType type;
    union
    {
        IEventKeyPress keyPress;
        IEventClick mouseClick;
        IEventMouseMove mouseMove;
    };
};



class InputManager
{

public:
    InputManager();
    ~InputManager() = default;

    void input(SDL_Event& evt);

    std::vector<IEvent>& queue();

    bool isHeld(SDL_Keycode keycode) const;
    bool anyHeld() const;

private:

    bool m_anyHeld;
    std::vector<IEvent> m_queue;
    std::unordered_set<SDL_Keycode> m_heldKeys;

};