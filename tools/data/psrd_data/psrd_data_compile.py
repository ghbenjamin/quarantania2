import json, pathlib, re
from psrd_orm import *

ROOT_PSRF = pathlib.Path("../psrd_data_modified").absolute().resolve()

def import_items():

    items = []
    weapons = []

    for fpath in ROOT_PSRF.joinpath("core_rulebook").joinpath("item").rglob( "*.json" ):
        f_json = json.loads( fpath.read_text() )
        try:

            itemdata = ItemData()

            # Name
            itemdata.item_name = f_json["name"]
            name_comma_re = re.search( r"([^,]+),([^,]+)", itemdata.item_name )
            if name_comma_re:
                itemdata.item_name = f"{ name_comma_re.group(2) } { name_comma_re.group(1) }".strip()

            # Weight
            try:
                itemdata.weight = int( re.search("(\d+)", f_json["weight"]).group(0) )
            except:
                itemdata.weight = 0

            # Price
            if "price" in f_json:
                price_str = f_json["price"].replace(",", "")
                price_re = re.match( "(\d+) ([csgp])p", price_str )
                if price_re:
                    price_num = int(price_re.group(1))
                    price_unit = price_re.group(2)
                    if price_unit == "s":
                        price_num *= 10
                    elif price_unit == "g":
                        price_num *= 100
                    elif price_unit == "p":
                        price_num *= 1000
                    itemdata.value = price_num
                else:
                    itemdata.value = 0
            else:
                itemdata.value = 0

            # Description
            if "body" in f_json:
                itemdata.description = f_json["body"]
            elif "sections" in f_json and "body" in f_json["sections"][0]:
                itemdata.description = f_json["sections"][0]["body"]

            # Aura

            if "aura" in f_json:
                itemdata.aura = f_json['aura']

            # Slot

            if "slot" in f_json and f_json["slot"] != "none":
                itemdata.slot = f_json["slot"]

            items.append(itemdata)

            if "misc" in f_json:
                if "Weapon" in f_json["misc"]:
                    weapon_json = f_json["misc"]["Weapon"]
                    weapon_obj = WeaponData()
                    weapon_obj.item_name = itemdata.item_name
                    weapon_obj.weapon_class = weapon_json["Weapon Class"].replace(" Weapons", "")
                    weapon_obj.proficiency = weapon_json["Proficiency"]

                    if "Type" in weapon_json:
                        weapon_obj.type = weapon_json["Type"]

                    if "Critical" in weapon_json:
                        weapon_obj.critical = weapon_json["Critical"].replace("&ndash;", "-").replace("&times;", "x")
                    else:
                        weapon_obj.critical = "x2"

                    if "Dmg (M)" in weapon_json:
                        weapon_obj.damage = weapon_json["Dmg (M)"]

                    if "Special" in weapon_json:
                        weapon_obj.specials = weapon_json["Special"]

                    weapons.append(weapon_obj)


        except:
            print(f_json)
            raise

    session = Session()
    session.bulk_save_objects(items)
    session.bulk_save_objects(weapons)
    session.commit()



def import_creatures():
    creatures = []
    for fpath in ROOT_PSRF.joinpath("bestiary").joinpath("creature").rglob( "*.json" ):

        f_json = json.loads( fpath.read_text() )
        try:

            cdata = CreatureData()
            cdata.name = " ".join( [i.strip() for i in reversed(f_json["name"].split(","))] )

            if "description" in f_json:
                cdata.description = f_json["description"]

            try:
                cdata.attr_str = int( f_json["strength"] )
            except:
                cdata.attr_str = 10

            cdata.attr_dex = int( f_json["dexterity"] )

            try:
                cdata.attr_con = int( f_json["constitution"] )
            except:
                cdata.attr_con = 10

            try:
                cdata.attr_int = int( f_json["intelligence"] )
            except:
                cdata.attr_int = 0

            cdata.attr_wis = int( f_json["wisdom"] )
            cdata.attr_cha = int( f_json["charisma"] )

            cdata.xp = int( f_json["xp"].replace(",", "") )
            cdata.alignment = f_json["alignment"]
            cdata.size = f_json["size"]


            def parse_save( save ):
                try:
                    return int( f_json[save] )
                except:
                    return int( re.search( r"([+-]\d+)", f_json[save] ).group(1) )

            cdata.save_fort = parse_save("fortitude")
            cdata.save_ref = parse_save("reflex")
            cdata.save_will = parse_save("will")

            cdata.hp = int( re.search( r"^(\d+) ", f_json["hp"] ).group(1) )

            if "reach" in f_json:
                cdata.reach = int( re.search( r"^(\d+) ft", f_json["reach"] ).group(1) )

            # Speed
            speed_str = f_json["speed"]
            speed_land_re = re.match( r"^(\d+) ft", speed_str )
            if speed_land_re:
                cdata.speed_land = int( speed_land_re.group(1) )
            else:
                cdata.speed_land = 0

            speed_fly_re = re.match( r"^fly (\d+) ft", speed_str )
            if speed_fly_re:
                cdata.speed_fly = int( speed_fly_re.group(1) )
            else:
                cdata.speed_fly = 0

            speed_swim_re = re.match( r"^swim (\d+) ft", speed_str )
            if speed_swim_re:
                cdata.speed_swim = int( speed_swim_re.group(1) )
            else:
                cdata.speed_swim = 0


            cdata.cmb = int( re.match( r"^([+-]?\d+)", f_json["cmb"] ).group(1) )
            cdata.cmd = int( re.match( r"^([+-]?\d+)", f_json["cmd"] ).group(1) )

            print( f_json["senses"])


            creatures.append( cdata )

        except:
            print(f_json)
            raise

    session = Session()
    session.bulk_save_objects(creatures)
    session.commit()


def main():
    import_items()
    import_creatures()


if __name__ == "__main__":
    main()