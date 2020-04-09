from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import create_engine
from sqlalchemy import Column
from sqlalchemy.orm import sessionmaker
from sqlalchemy.dialects.sqlite import TEXT, INTEGER, CHAR

Base = declarative_base()

class WeaponData(Base):
    __tablename__ = "weapon_data"

    item_name = Column(CHAR(64), primary_key=True)
    weapon_class = Column(TEXT, nullable=False)
    damage = Column(TEXT)
    proficiency = Column(TEXT, nullable=False)
    crit_lower = Column(INTEGER)
    crit_mult = Column(INTEGER)
    damage_type = Column(TEXT)
    specials = Column(TEXT)

class ArmourData(Base):
    __tablename__ = "armour_data"

    item_name = Column(CHAR(64), primary_key=True)
    arcane_failure_chance = Column(INTEGER)
    armour_bonus = Column(INTEGER)
    shield_bonus = Column(INTEGER)
    armour_check_penalty = Column(INTEGER)
    max_dex = Column(INTEGER)
    speed_20 = Column(INTEGER)
    speed_30 = Column(INTEGER)
    armour_type = Column(TEXT)

class ItemData(Base):
    __tablename__ = "item_data"

    item_name = Column(CHAR(64), primary_key=True)
    weight = Column(INTEGER, nullable=False)
    value = Column(INTEGER, nullable=False)
    description = Column(TEXT, nullable=False)
    aura_strength = Column(TEXT)
    aura_value = Column(TEXT)
    slot = Column(TEXT)
    item_type = Column(TEXT, nullable=False)
    sprite_sheet = Column(TEXT, nullable=False)
    sprite_name = Column(TEXT, nullable=False)

class CreatureData(Base):
    __tablename__ = "creature_data"

    name = Column(CHAR(64), primary_key=True)
    description = Column(TEXT)

    xp = Column(INTEGER, nullable=False)
    hp = Column(INTEGER, nullable=False)
    alignment = Column(TEXT, nullable=False)
    size = Column(TEXT, nullable=False)

    creature_type = Column(TEXT, nullable=False)
    creature_subtype = Column(TEXT)

    save_fort = Column(INTEGER, nullable=False)
    save_ref = Column(INTEGER, nullable=False)
    save_will = Column(INTEGER, nullable=False)

    attr_str = Column(INTEGER, nullable=False)
    attr_dex = Column(INTEGER, nullable=False)
    attr_con = Column(INTEGER, nullable=False)
    attr_int = Column(INTEGER, nullable=False)
    attr_wis = Column(INTEGER, nullable=False)
    attr_cha = Column(INTEGER, nullable=False)

    reach = Column(INTEGER)

    speed_land = Column(INTEGER)
    speed_fly = Column(INTEGER)
    speed_swim = Column(INTEGER)

    cmd = Column(INTEGER, nullable=False)
    cmb = Column(INTEGER, nullable=False)
    dr = Column(TEXT)
    resist = Column(TEXT)
    immune = Column(TEXT)

    sprite_sheet = Column(TEXT, nullable=False)
    sprite_name = Column(TEXT, nullable=False)

engine = create_engine('sqlite+pysqlite:///../../../resource/data/psrd_data.db')
Session = sessionmaker(bind=engine)

Base.metadata.drop_all(engine)
Base.metadata.create_all(engine)
