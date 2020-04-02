#include <db/ResourceDatabase.h>
#include <exception>
#include <utils/Logging.h>

ResourceDatabase::ResourceDatabase()
{
    int ierr = sqlite3_open( "../resource/content.db", &m_db );
    if ( ierr != SQLITE_OK )
    {
        Logging::log( "FATAL ERROR: Unable to open content database" );
        std::terminate();
    }
}

ResourceDatabase::~ResourceDatabase()
{
    sqlite3_close( m_db );
}
