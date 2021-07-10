#include <state/InitState.h>
#include <game/LevelContext.h>
#include <game/RunState.h>
#include <state/LevelState.h>
#include <state/OverworldState.h>

InitState::InitState() {}

bool InitState::input(IEvent &evt)
{
    return false;
}

void InitState::update(uint32_t ticks, InputInterface &iinter, RenderInterface &rInter)
{
    auto runState = generateNewRun();
    auto serialized = runState->serialize();
    Logging::log( serialized.dump(1) );

    // LEVEL START
    auto ctx = std::make_shared<LevelContext>();
    setNextState<LevelState>( ctx, runState );

    
//    // OVERWORLD START
//    setNextState<OverworldState>();
}

std::shared_ptr<RunState> InitState::generateNewRun()
{
    auto state = std::make_shared<RunState>();

    PlayerData p1;
    p1.name = "Angus MacKenzie";
    p1.alignment = DnDAlignment::TN;
    p1.playerRace = PlayerRace::Human;
    p1.playerClass = PlayerClass::Fighter;
    p1.sprite = "dawnlike_chars/Player_001";
    p1.maxHP = 12;

    p1.attrStr = 300;
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

    state->playerChars = { p1, p2 };

    return state;
}


