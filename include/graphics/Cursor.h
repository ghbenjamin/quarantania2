#pragma once

#include <memory>


struct SDL_Cursor;

enum class CursorType
{
    Default,
    Interact,
    Attack
};


class CursorWrapper
{
public:
    CursorWrapper( CursorType type );
    ~CursorWrapper();

    SDL_Cursor* getCursor();

private:
    SDL_Cursor* m_cursor;
};


class Cursor
{
public:
    Cursor();
    ~Cursor() = default;

    void setCursorType( CursorType type );
    void resetCursor();

private:
    CursorType m_currentType;
    std::unique_ptr<CursorWrapper> m_cursor;
};