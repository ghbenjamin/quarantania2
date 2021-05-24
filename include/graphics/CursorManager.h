#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include "Primatives.h"

struct SDL_Cursor;
class Surface;

enum class CursorType
{
    Default,
    Interact,
    Attack,
    Move,
    Examine
};


class CursorWrapper
{
public:
    CursorWrapper( std::shared_ptr<Surface> surface, Vector2i hotspot );
    ~CursorWrapper();

    SDL_Cursor* getCursor();

private:
    SDL_Cursor* m_cursor;
};


class CursorManager
{
public:
    CursorManager();
    ~CursorManager() = default;
    
    void loadCursors();
    void addCursor( CursorType type, std::string cursorName, Vector2i hotspot );
    
    void setCursorType( CursorType type, bool force = false );
    void resetCursor();

private:
    std::unordered_map<CursorType, std::shared_ptr<CursorWrapper>> m_cursors;
    CursorType m_currentType;
};