#pragma once

#include <utils/Json.h>
#include <game/Defines.h>
#include <resource/Spritesheet.h>
#include <game/RawData.h>

struct ActorData
{
    // Meta
    SpritesheetKey sprite;
    std::string name;
    DnDAlignment alignment;
    CreatureSize size = CreatureSize::Medium;
    int level = 1;
    int xp;
    
    std::string creatureType;
    std::vector<std::string> creatureSubtypes;
    
    // Health
    int maxHP;
    int currHP;
    
    // Attributes
    int attrStr = 10;
    int attrDex = 10;
    int attrCon = 10;
    int attrInt = 10;
    int attrWis = 10;
    int attrCha = 10;
    
    // Saves
    int saveFort = 0;
    int saveRef = 0;
    int saveWill = 0;
    
    int baseSpeed = 6;
    
    
    std::vector<std::string> featIds;
    std::vector<std::string> heldItems;
    std::vector<std::string> equippedItems;
};


struct PlayerData : public ActorData
{
    utils::json::object serialize() const;
    
    std::string playerClass;
};

struct CreatureData : public ActorData
{
    std::string description;
    
    std::unordered_map<std::string, int> damageResistance;
    std::unordered_map<ElementalDamageType, int> elementalResistance;
    std::vector<ElementalDamageType> weaknesses;
    std::vector<std::string> senses;
    std::vector<std::string> immune;
    
    std::vector<CreatureAttack> attacks;
};