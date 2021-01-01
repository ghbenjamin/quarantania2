#include <graphics/Cursor.h>
#include <SDL2/SDL.h>

Cursor::Cursor()
    : m_currentType(CursorType::Default)
{}

void Cursor::setCursorType(CursorType type)
{
    m_cursor = std::make_unique<CursorWrapper>(type);

    SDL_SetCursor( m_cursor->getCursor() );
}

void Cursor::resetCursor()
{
    setCursorType( CursorType::Default );
}


CursorWrapper::CursorWrapper(CursorType type)
{
    SDL_SystemCursor sdlCursorType;

    switch (type)
    {
        case CursorType::Attack:
            sdlCursorType = SDL_SYSTEM_CURSOR_CROSSHAIR;
            break;
        case CursorType::Interact:
            sdlCursorType = SDL_SYSTEM_CURSOR_HAND;
            break;
        default:
            sdlCursorType = SDL_SYSTEM_CURSOR_ARROW;
            break;
    }

    m_cursor = SDL_CreateSystemCursor(sdlCursorType);
}

CursorWrapper::~CursorWrapper()
{
    SDL_FreeCursor( m_cursor );
    m_cursor = nullptr;
}

SDL_Cursor *CursorWrapper::getCursor()
{
    return m_cursor;
}
