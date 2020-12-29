
from pathlib import Path
import json, sys, pprint, re


OLD_JSON_PATH = Path("../../resource/data/creatures_old.json")
NEW_JSON_PATH = Path("../../resource/data/creatures.json")

debug_set = set()


def main():

    old_json = json.loads( OLD_JSON_PATH.read_text() )
    new_json = []

    for item in old_json:
        try:
            new_item = {}

            if "ac" in item:
                del(item["ac"])

            new_item["attr_str"] = item["strength"]
            del(item["strength"])
            new_item["attr_dex"] = item["dexterity"]
            del(item["dexterity"])
            new_item["attr_con"] = item["constitution"]
            del(item["constitution"])
            new_item["attr_int"] = item["intelligence"]
            del(item["intelligence"])
            new_item["attr_wis"] = item["wisdom"]
            del(item["wisdom"])
            new_item["attr_cha"] = item["charisma"]
            del(item["charisma"])

            new_item["save_fort"] = item["fortitude"]
            del(item["fortitude"])
            new_item["save_ref"] = item["reflex"]
            del(item["reflex"])
            new_item["save_will"] = item["will"]
            del(item["will"])

            new_item["initative"] = item["init"]
            del(item["init"])

            new_item["sprite"] = f"{item['sprite_sheet']}/{item['sprite_name']}"
            del(item['sprite_name'])
            del(item['sprite_sheet'])


            if "creature_subtype" in item:
                subtypes = item["creature_subtype"].split(",")
                new_item["creature_subtypes"] = [i.strip() for i in subtypes]
                del(item["creature_subtype"])

            for k in ["alignment", "creature_type", "hp", "base_attack", "description",
                "xp", "cmb", "cmd", "name", "size", "ranged", "melee", "special_attacks",
                "spell_like_abilities", "spells_prepared", "spells_known", "reach"]:
                if k in item:
                    new_item[k] = item[k]
                    del(item[k])

            if "resist" in item:
                resists = item["resist"].split(",")
                res_map = {}
                for i in resists:
                    k, v = i.split()
                    res_map[k.strip()] = int(v.strip())
                new_item["elemental_resistance"] = res_map
                del(item["resist"])


            if "level" in item:
                del(item["level"])

            if "sex" in item:
                del(item["sex"])

            if "gear" in item:
                del(item["gear"])

            if "speed" in item:
                m = re.search( r"(\d+) ft", item["speed"] )
                if m:
                    new_item["speed"] = int(m.group(1)) // 5
                else:
                    raise
                del(item["speed"])

            if "senses" in item:
                senses = item["senses"]
                senses_arr = []
                if "darkvision" in senses:
                    senses_arr.append("darkvision")
                if "low-light vision" in senses:
                    senses_arr.append("low-light")
                if "true seeing" in senses:
                    senses_arr.append("true-seeing")
                if "blindsense" in senses:
                    senses_arr.append("blindsense")
                if "scent" in senses:
                    senses_arr.append("scent")
                if "tremorsense" in senses:
                    senses_arr.append("tremorsense")

                if len(senses_arr):
                    new_item["senses"] = senses_arr
                del(item["senses"])

            if "feats" in item:
                feat_arr = item["feats"].split(",")
                new_item["feats"] = [i.strip() for i in feat_arr]
                del(item["feats"])

            if "languages" in item:
                del(item["languages"])

            if "space" in item:
                del(item["space"])

            if 'defensive_abilities' in item:
                del(item['defensive_abilities'])

            if "dr" in item:
                dr_arr = item["dr"].split(";")
                dr_map = {}
                for dr in dr_arr:
                    k, v = dr.split("/")
                    dr_map[v] = int(k)
                new_item["damage_resistance"] = dr_map
                del(item["dr"])

            if "immune" in item:
                immunes = item["immune"].split(",")
                new_item["immune"] = [i.strip() for i in immunes]
                del(item["immune"])

            if "sr" in item:
                new_item["spell_resistance"] = int(item["sr"])
                del(item["sr"])

            if 'special_qualities' in item:
                del(item['special_qualities'])

            if 'weaknesses' in item:
                weak_item = item['weaknesses']
                weak_arr = []
                for i in ["fire", "cold", "electricity"]:
                    if i in weak_item:
                        weak_arr.append(i)
                if len(weak_arr):
                    new_item['weaknesses'] = weak_arr
                del(item['weaknesses'])

            if 'super_race' in item:
                del(item['super_race'])

            # if len(item) != 0:
            #     pprint.pprint(item)
            #     sys.exit()

            new_json.append(new_item)

        except:
            # pprint.pprint(item)
            raise


    NEW_JSON_PATH.write_text( json.dumps(new_json, indent=4, sort_keys=True) )


if __name__ == "__main__":
    main()