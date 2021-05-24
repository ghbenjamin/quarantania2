#include <graphics/CursorManager.h>
#include <SDL2/SDL.h>
#include <resource/ResourceManager.h>
#include <fmt/format.h>


// Cursor manager
// ----------------------

CursorManager::CursorManager()
 : m_currentType(CursorType::Default) {}

void CursorManager::setCursorType( CursorType type, bool force )
{
    if ( !force && type == m_currentType )
    {
        return;
    }

    SDL_Cursor* cursor = m_cursors[type]->getCursor();
    SDL_SetCursor( cursor );
    m_currentType = type;
}

void CursorManager::resetCursor()
{
    setCursorType( CursorType::Default );
}

void CursorManager::addCursor( CursorType type, std::string cursorName, Vector2i hotspot )
{
    auto const& surface = ResourceManager::get().getSurface( cursorName );
    m_cursors.emplace( type, std::make_shared<CursorWrapper>( surface, hotspot ) );
}

void CursorManager::loadCursors()
{
    // Add our various cursor types
    addCursor(CursorType::Default, "default-cursor", { 0, 0 });
    addCursor(CursorType::Interact, "interact-cursor", { 8, 0 });
    addCursor(CursorType::Attack, "attack-cursor", { 0, 0 });
    addCursor(CursorType::Move, "move-cursor", { 0, 0 });
    addCursor(CursorType::Examine, "examine-cursor", { 0, 0 });
    
    // Change from the initial system cursor to our default custom cursor
    setCursorType(CursorType::Default, true);
}



// Cursor wrapper
// ----------------------

CursorWrapper::CursorWrapper(std::shared_ptr<Surface> surface, Vector2i hotspot)
{
    m_cursor = SDL_CreateColorCursor( surface->raw(), hotspot.x(), hotspot.y() );
    
    if (m_cursor == NULL)
    {
        AssertAlwaysMsg( fmt::format( "Error creating SDL surface from stbi data", SDL_GetError() ) );
    }
}

CursorWrapper::~CursorWrapper()
{
    SDL_FreeCursor( m_cursor );
}

SDL_Cursor *CursorWrapper::getCursor()
{
    return m_cursor;
}
