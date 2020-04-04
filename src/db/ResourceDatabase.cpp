#include <db/ResourceDatabase.h>
#include <exception>
#include <utils/Logging.h>
#include <game/Items.h>
#include <utils/Assert.h>

ResourceDatabase::ResourceDatabase()
{
    int ierr = sqlite3_open_v2( "../resource/data/psrd_data.db", &m_db, SQLITE_OPEN_READONLY, NULL );
    if ( ierr != SQLITE_OK )
    {
        Logging::log( "FATAL ERROR: Unable to open content database" );
        std::terminate();
    }


    sqlite3_prepare_v2( m_db, "SELECT * FROM item_data WHERE item_data.item_name=?", -1, &m_itemByNameQuery, NULL );


}

ResourceDatabase::~ResourceDatabase()
{
    sqlite3_close( m_db );
}

std::shared_ptr<Item> ResourceDatabase::itemFromName(std::string_view name)
{
    std::string item_name = std::string(name);

    ItemType item_type;
    int item_value;
    int item_weight;
    SpritesheetKey item_sprite;
    EquipSlotMask item_equip = 0;
    std::string item_desc;

    int status;
    sqlite3_bind_text( m_itemByNameQuery, 1, item_name.c_str(), -1, NULL );

    status = sqlite3_step( m_itemByNameQuery );
    AssertMsg( status == SQLITE_ROW, "Unknown item" );

    item_weight = sqlite3_column_int( m_itemByNameQuery, 1 );
    item_value = sqlite3_column_int( m_itemByNameQuery, 2 );
    item_desc = std::string(reinterpret_cast<const char *const>(sqlite3_column_text(m_itemByNameQuery, 3)));
    item_sprite = { "dawnlike_items", "LongWep_028" };

    sqlite3_reset( m_itemByNameQuery );

    return std::make_shared<Item>( item_name, item_type, item_value, item_weight, item_sprite, item_equip );
}
