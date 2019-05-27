#include <game/InputInterface.h>
#include <utils/Logging.h>

InputInterface::InputInterface()
: m_anyHeld(false)
{

}

void InputInterface::input(SDL_Event &sdlEvent)
{
    switch ( sdlEvent.type )
    {
        case SDL_KEYDOWN:
        {
            auto& key = sdlEvent.key.keysym.sym;
            m_heldKeys.insert(key);
            m_anyHeld = true;

            IEvent evt;
            evt.type = IEventType::KeyPress;
            evt.keyPress = IEventKeyPress{ key };

            m_queue.push_back(evt);
            break;
        }

        case SDL_KEYUP:
        {
            auto& key = sdlEvent.key.keysym.sym;
            if ( auto it = m_heldKeys.find(key); it != m_heldKeys.end() )
            {
                m_heldKeys.erase(it);
            }

            if (m_heldKeys.empty())
            {
                m_anyHeld = false;
            }
            break;
        }

        case SDL_MOUSEBUTTONDOWN:
        {
            Vector2i screenPos = { sdlEvent.button.x, sdlEvent.button.y };

            IEvent evt;
            evt.type = IEventType::MouseClick;
            evt.mouseClick = {
                screenPos,
                sdlEvent.button.button
            };

            m_queue.push_back(evt);
            break;
        }

        case SDL_MOUSEBUTTONUP:
        {
            break;
        }

        case SDL_MOUSEMOTION:
        {
            Vector2i screenPos = { sdlEvent.button.x, sdlEvent.button.y };

            IEvent evt;
            evt.type = IEventType::MouseMove;

            evt.mouseMove = {
                screenPos,
            };

            m_queue.push_back(evt);
            break;
        }

        case SDL_WINDOWEVENT:
        {
            switch (sdlEvent.window.event)
            {
                case SDL_WINDOWEVENT_RESIZED:
                {
                    IEvent evt;
                    evt.type = IEventType::WindowResize;

                    evt.windowResize = Vector2i{ sdlEvent.window.data1, sdlEvent.window.data2 };

                    m_queue.push_back(evt);
                    break;
                }

                default:
                {
                    break;
                }
            }
        }

        default:
        {
            break;
        }
    }
}

std::vector<IEvent> &InputInterface::queue()
{
    return m_queue;
}

bool InputInterface::isHeld(SDL_Keycode keycode) const
{
    return m_heldKeys.find(keycode) != m_heldKeys.end();
}

bool InputInterface::anyHeld() const
{
    return m_anyHeld;
}

void InputInterface::clear()
{
    m_queue.clear();
}

IEventKeyPress::IEventKeyPress()
{

}

IEventKeyPress::IEventKeyPress(SDL_Keycode key)
: keyCode(key)
{

}

IEventClick::IEventClick()
: screenPos{0, 0}, button(-1)
{

}

IEventClick::IEventClick(Vector2i sp, int btn)
: screenPos(sp), button(btn)
{

}

IEventMouseMove::IEventMouseMove()
: screenPos{0, 0}
{

}

IEventMouseMove::IEventMouseMove(Vector2i sp)
: screenPos(sp)
{

}

IEvent::IEvent()
{

}

IEventWindowResize::IEventWindowResize()
: screenSize{0, 0}
{

}

IEventWindowResize::IEventWindowResize(Vector2i size)
: screenSize(size)
{

}
