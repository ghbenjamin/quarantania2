#pragma once

#include <game/Entity.h>
#include <resource/Spritesheet.h>

class Level;

enum class PrefabType
{
    Door,
    Exit,
    Entrance,
    Container,
    Decor,
};

PrefabType prefabTypeFromName(std::string const& name);


class PrefabObj
{
public:
    PrefabObj() = default;
    virtual ~PrefabObj() = default;

    virtual void generate(Level *level, EntityRef entity) = 0;
};

namespace PrefabObjs
{

class Door : public PrefabObj
{
public:
    Door( SpritesheetKey sprite );
    ~Door() override = default;
    void generate(Level* level, EntityRef entity) override;
private:
    SpritesheetKey m_sprite;
};

class Exit : public PrefabObj
{
public:
    Exit( SpritesheetKey sprite );
    ~Exit() override = default;
    void generate(Level* level, EntityRef entity) override;
private:
    SpritesheetKey m_sprite;
};

class Entrance : public PrefabObj
{
public:
    Entrance( SpritesheetKey sprite );
    ~Entrance() override = default;
    void generate(Level* level, EntityRef entity) override;
private:
    SpritesheetKey m_sprite;
};

class Container : public PrefabObj
{
public:
    Container( SpritesheetKey sprite );
    ~Container() override = default;
    void generate(Level* level, EntityRef entity) override;
private:
    SpritesheetKey m_sprite;
};

class Decor : public PrefabObj
{
public:
    Decor( SpritesheetKey sprite );
    ~Decor() override = default;
    void generate(Level* level, EntityRef entity) override;
private:
    SpritesheetKey m_sprite;
};


}
