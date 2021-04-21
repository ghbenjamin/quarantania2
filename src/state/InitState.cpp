#include <state/InitState.h>
#include <game/Player.h>
#include <game/LevelContext.h>
#include <state/LevelState.h>

InitState::InitState()
: m_levelCtx( std::make_shared<LevelContext>() )
{
}

bool InitState::input(IEvent &evt)
{
    return false;
}

void InitState::update(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    m_levelCtx->depth++;

    PlayerData p1;
    p1.name = "Angus MacKenzie";
    p1.alignment = DnDAlignment::TN;
    p1.playerRace = PlayerRace::Human;
    p1.playerClass = PlayerClass::Fighter;
    p1.sprite = "dawnlike_chars/Player_001";
    p1.maxHP = 12;

    p1.attrStr = 16;
    p1.attrDex = 12;
    p1.attrCon = 14;
    p1.attrInt = 9;
    p1.attrWis = 10;
    p1.attrCha = 12;

    p1.startingEquippedItems.emplace_back( "Longsword" );
    p1.featIds.push_back( "feat.power_attack" );


    PlayerData p2;
    p2.name = "Gwendlyn Di Corci";
    p2.alignment = DnDAlignment::TN;
    p2.playerRace = PlayerRace::Human;
    p2.playerClass = PlayerClass::Fighter;
    p2.sprite = "dawnlike_chars/Player_003";
    p2.maxHP = 9;

    p2.attrStr = 16;
    p2.attrDex = 12;
    p2.attrCon = 14;
    p2.attrInt = 9;
    p2.attrWis = 10;
    p2.attrCha = 12;

    p2.startingEquippedItems.emplace_back( "Scimitar" );


    PartyData pdata;
    pdata.playerChars = { p1, p2 };

    setNextState<LevelState>( m_levelCtx, pdata );
}


