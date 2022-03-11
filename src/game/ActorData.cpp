#include <game/ActorData.h>

utils::json::object PlayerData::serialize() const
{
    utils::json::object obj;
    
    obj["name"] = name;
    obj["curr_xp"] = currHP;
    obj["max_hp"] = maxHP;
    obj["curr_hp"] = currHP;
    
    obj["attributes"] = { attrStr, attrDex, attrCon, attrInt, attrWis, attrCha };
    obj["saves"] = { saveFort, saveRef, saveWill };
    
    obj["held_items"] = utils::json::object::array();
    for ( auto const& i : heldItems )
    {
        obj["held_items"].push_back(i);
    }
    
    obj["equipped_items"] = utils::json::object::array();
    for ( auto const& i : equippedItems )
    {
        obj["equipped_items"].push_back(i);
    }
    
    obj["feats"] = utils::json::object::array();
    for ( auto const& i : featIds )
    {
        obj["feats"].push_back(i);
    }
    
    return obj;
}
