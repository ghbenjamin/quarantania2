from sqlalchemy.ext.declarative import declarative_base
from sqlalchemy import create_engine
from sqlalchemy import Column
from sqlalchemy.orm import sessionmaker
from sqlalchemy.dialects.sqlite import TEXT, INTEGER, CHAR

Base = declarative_base()

class WeaponData(Base):
    __tablename__ = "weapon_data"

    item_name = Column(CHAR(64), primary_key=True)
    weapon_class = Column(TEXT)
    damage = Column(TEXT)
    proficiency = Column(TEXT)
    critical = Column(TEXT)
    type = Column(TEXT)
    specials = Column(TEXT)

class ItemData(Base):
    __tablename__ = "item_data"

    item_name = Column(CHAR(64), primary_key=True)
    weight = Column(INTEGER)
    value = Column(INTEGER)
    description = Column(TEXT)
    aura = Column(TEXT)
    slot = Column(TEXT)

class CreatureData(Base):
    __tablename__ = "creature_data"

    name = Column(CHAR(64), primary_key=True)
    description = Column(TEXT)

    xp = Column(INTEGER)
    hp = Column(INTEGER)
    alignment = Column(TEXT)
    size = Column(TEXT)

    save_fort = Column(INTEGER)
    save_ref = Column(INTEGER)
    save_will = Column(INTEGER)

    attr_str = Column(INTEGER)
    attr_dex = Column(INTEGER)
    attr_con = Column(INTEGER)
    attr_int = Column(INTEGER)
    attr_wis = Column(INTEGER)
    attr_cha = Column(INTEGER)

    reach = Column(INTEGER)

    speed_land = Column(INTEGER)
    speed_fly = Column(INTEGER)
    speed_swim = Column(INTEGER)

    cmd = Column(INTEGER)
    cmb = Column(INTEGER)


engine = create_engine('sqlite+pysqlite:///../psrd_data.db')
Session = sessionmaker(bind=engine)

Base.metadata.drop_all(engine)
Base.metadata.create_all(engine)
