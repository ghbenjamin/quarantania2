import json, pathlib, re
from psrd_orm import *

ROOT_FINAL_PSRF = pathlib.Path("../psrd_final").absolute().resolve()

def import_items():

    items = []
    weapons = []
    armour = []

    all_json = json.loads( ROOT_FINAL_PSRF.joinpath("items.json").read_text() )
    for f_json in all_json:

        try:
            itemdata = ItemData()

            itemdata.item_name = f_json["name"]
            itemdata.weight = f_json["weight"]
            itemdata.value = f_json["price"]
            itemdata.description = f_json["description"]
            itemdata.item_type = f_json["item_type"]
            itemdata.sprite_sheet = f_json["sprite_sheet"]
            itemdata.sprite_name = f_json["sprite_name"]

            if "slot" in f_json:
                itemdata.slot = f_json["slot"]
            if "aura_strength" in f_json:
                itemdata.aura_strength = f_json["aura_strength"]
            if "aura_value" in f_json:
                itemdata.aura_value = f_json["aura_value"]

            items.append(itemdata)

            if "armour" in f_json:
                armour_json = f_json["armour"]
                armour_obj = ArmourData()
                armour_obj.item_name = itemdata.item_name

                if "Arcane Spell Failure Chance" in armour_json:
                    armour_obj.arcane_failure_chance = armour_json["Arcane Spell Failure Chance"]
                if "Armor Bonus" in armour_json:
                    armour_obj.armour_bonus = armour_json["Armor Bonus"]
                if "Shield Bonus" in armour_json:
                    armour_obj.shield_bonus = armour_json["Shield Bonus"]
                if "Armor Check Penalty" in armour_json:
                    armour_obj.armour_check_penalty = armour_json["Armor Check Penalty"]
                if "Armor Type" in armour_json:
                    armour_obj.armour_type = armour_json["Armor Type"]
                if "Maximum Dex Bonus" in armour_json:
                    armour_obj.max_dex = armour_json["Maximum Dex Bonus"]
                if "Speed 20" in armour_json:
                    armour_obj.speed_20 = armour_json["Speed 20"]
                if "Speed 30" in armour_json:
                    armour_obj.speed_30 = armour_json["Speed 30"]

                armour.append(armour_obj)

            if "weapon" in f_json:
                weapon_json = f_json["weapon"]
                weapon_obj = WeaponData()
                weapon_obj.item_name = itemdata.item_name
                weapon_obj.weapon_class = weapon_json["weapon_class"]
                weapon_obj.proficiency = weapon_json["proficiency"]
                weapon_obj.crit_mult = weapon_json["crit_mult"]
                weapon_obj.crit_lower = weapon_json["crit_lower"]

                if "damage_type" in weapon_json:
                    weapon_obj.damage_type = weapon_json["damage_type"]

                if "damage" in weapon_json:
                    weapon_obj.damage = weapon_json["damage"]

                if "Special" in weapon_json:
                    weapon_obj.specials = weapon_json["Special"]

                weapons.append(weapon_obj)

        except:
            print(f_json)
            raise

    session = Session()
    session.bulk_save_objects(items)
    session.bulk_save_objects(weapons)
    session.bulk_save_objects(armour)
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
            cdata.alignment = f_json["alignment"]
            cdata.creature_type = f_json["creature_type"]
            cdata.hp = f_json["hp"]
            cdata.sprite_sheet = f_json["sprite_sheet"]
            cdata.sprite_name = f_json["sprite_name"]

            if "creature_subtype" in f_json:
                cdata.creature_subtype = f_json["creature_subtype"]
            if "reach" in f_json:
                cdata.reach = f_json["reach"]
            if "dr" in f_json:
                cdata.dr = f_json["dr"]
            if "resist" in f_json:
                cdata.resist = f_json["resist"]
            if "immune" in f_json:
                cdata.immune = f_json["immune"]

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