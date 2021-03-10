#include <game/GameEventHub.h>

GameEventHub::GameEventHub()
{
    
    // Uncomment me for a reference to help with debugging event issues
//    Logging::log("Game Event Def IDs:");
//    Logging::log( "EntityMove ID: {}\n", GameEvent<GameEvents::EntityMove>::id() );
//    Logging::log( "EntityReady ID: {}\n", GameEvent<GameEvents::EntityReady>::id() );
//    Logging::log( "EntityOpenClose ID: {}\n", GameEvent<GameEvents::EntityOpenClose>::id() );
//    Logging::log( "LevelReady ID: {}\n", GameEvent<GameEvents::LevelReady>::id() );
//    Logging::log( "TurnChange ID: {}\n", GameEvent<GameEvents::TurnChange>::id() );
//    Logging::log( "RoundChange ID: {}\n", GameEvent<GameEvents::RoundChange>::id() );
//    Logging::log( "EntityAction ID: {}\n", GameEvent<GameEvents::EntityAction>::id() );
//    Logging::log( "ItemPickup ID: {}\n", GameEvent<GameEvents::ItemPickup>::id() );
//    Logging::log( "ItemDrop ID: {}\n", GameEvent<GameEvents::ItemDrop>::id() );
//    Logging::log( "ItemEquip ID: {}\n", GameEvent<GameEvents::ItemEquip>::id() );
//    Logging::log( "ItemUnequip ID: {}\n", GameEvent<GameEvents::ItemUnequip>::id() );
//    Logging::log( "EntityDeath ID: {}\n", GameEvent<GameEvents::EntityDeath>::id() );
//    Logging::log( "EntityDamage ID: {}\n", GameEvent<GameEvents::EntityDamage>::id() );
//    Logging::log( "CombatMeleeAttack ID: {}\n", GameEvent<GameEvents::CombatMeleeAttack>::id() );
//    Logging::log( "ControllerEntitySelected ID: {}\n", GameEvent<GameEvents::ControllerEntitySelected>::id() );
}

void GameEventHub::popQueue()
{
    auto evt = m_evtQueue.back();
    m_evtQueue.pop();
    
    // All the subscribers for this event type, regardless of subscription timing
    auto it_range = m_subs.equal_range( evt.id );
    
    // For each subscription timing (before, on, after)
    for ( int i = 0; i < GEventTimingCount /* 3 */; i++ )
    {
        // Loop over each subscription three times, firing them once each in the correct order.
        for (auto it = it_range.first; it != it_range.second; it++)
        {
            int timing = (int)it->second.timing;
            if ( timing == i )
            {
                GEventSubBase* base = it->second.target;
                base->accept( evt.evt );
            }
        }
    }
}
