import json, pathlib, re
from psrd_orm import *

ROOT_FINAL_PSRF = pathlib.Path("../psrd_final").absolute().resolve()

def import_items():

    items = []
    weapons = []

    all_json = json.loads( ROOT_FINAL_PSRF.joinpath("items.json").read_text() )
    for f_json in all_json:

        try:
            itemdata = ItemData()

            itemdata.item_name = f_json["name"]
            itemdata.weight = f_json["weight"]
            itemdata.value = f_json["price"]
            itemdata.description = f_json["description"]
            itemdata.item_type = f_json["item_type"]

            if "slot" in f_json:
                itemdata.slot = f_json["slot"]
            if "aura_strength" in f_json:
                itemdata.aura_strength = f_json["aura_strength"]
            if "aura_value" in f_json:
                itemdata.aura_value = f_json["aura_value"]

            items.append(itemdata)

            if "weapon" in f_json:
                weapon_json = f_json["weapon"]
                weapon_obj = WeaponData()
                weapon_obj.item_name = itemdata.item_name
                weapon_obj.weapon_class = weapon_json["Weapon Class"].replace(" Weapons", "")
                weapon_obj.proficiency = weapon_json["Proficiency"]


            if "misc" in f_json:
                if "Weapon" in f_json["misc"]:
                    weapon_json = f_json["misc"]["Weapon"]
                    weapon_obj = WeaponData()
                    weapon_obj.item_name = itemdata.item_name
                    weapon_obj.weapon_class = weapon_json["Weapon Class"]
                    weapon_obj.proficiency = weapon_json["Proficiency"]

                    if "Type" in weapon_json:
                        weapon_obj.type = weapon_json["Type"]

                    if "Critical" in weapon_json:
                        weapon_obj.critical = weapon_json["Critical"].replace("&ndash;", "-").replace("&times;", "x")
                    else:
                        weapon_obj.critical = "x2"

                    if "Damage" in weapon_json:
                        weapon_obj.damage = weapon_json["Damage"]

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

    all_json = json.loads( ROOT_FINAL_PSRF.joinpath("creatures.json").read_text() )
    for f_json in all_json:

        try:
            cdata = CreatureData()
            cdata.name = f_json["name"]

            if "description" in f_json:
                cdata.description = f_json["description"]

            cdata.attr_str = f_json["strength"]
            cdata.attr_dex = f_json["dexterity"]
            cdata.attr_con = f_json["constitution"]
            cdata.attr_int = f_json["intelligence"]
            cdata.attr_wis = f_json["wisdom"]
            cdata.attr_cha = f_json["charisma"]
            cdata.xp = f_json["xp"]
            cdata.size = f_json["size"]
            cdata.save_fort = f_json["fortitude"]
            cdata.save_ref = f_json["reflex"]
            cdata.save_will = f_json["will"]
            cdata.cmb = f_json["cmb"]
            cdata.cmd = f_json["cmd"]


            if "reach" in f_json:
                cdata.reach = f_json["reach"]

            cdata.hp = int( re.search( r"^(\d+) ", f_json["hp"] ).group(1) )

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