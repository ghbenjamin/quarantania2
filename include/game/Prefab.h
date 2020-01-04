#pragma once

#include <game/Entity.h>
#include <resource/Spritesheet.h>

class Level;

enum class PrefabType
{
    Door,
    Door_Locked,

    Stairs_Up,
    Stairs_Down,

    Cont_Bookcase_Small,
    Cont_Bookcase_Large,
    Cont_Chest,
    Cont_Chest_Locked,
    Cont_Barrel,
    Cont_Crate,

    Decor_Bed,
    Decor_Fireplace,
    Decor_Chair
};

PrefabType prefabTypeFromName(std::string const& name);


class PrefabObj
{
public:
    PrefabObj() = default;
    virtual ~PrefabObj() = default;

    virtual void generate(Level *level, EntityRef entity) = 0;
};


class PrefabObjSprite : public PrefabObj
{
public:
    PrefabObjSprite( SpritesheetKey sprite );
    ~PrefabObjSprite() override = default;

protected:
    SpritesheetKey m_sprite;
};


namespace PrefabObjects
{

class Door : public PrefabObjSprite
{
public:
    using PrefabObjSprite::PrefabObjSprite;
    void generate(Level* level, EntityRef entity) override;
};

class Exit : public PrefabObjSprite
{
public:
    using PrefabObjSprite::PrefabObjSprite;
    void generate(Level* level, EntityRef entity) override;
};

class Entrance : public PrefabObjSprite
{
public:
    using PrefabObjSprite::PrefabObjSprite;
    void generate(Level* level, EntityRef entity) override;
};

class Container : public PrefabObjSprite
{
public:
    using PrefabObjSprite::PrefabObjSprite;
    void generate(Level* level, EntityRef entity) override;
};

class Decor : public PrefabObjSprite
{
public:
    using PrefabObjSprite::PrefabObjSprite;
    void generate(Level* level, EntityRef entity) override;
};

}
