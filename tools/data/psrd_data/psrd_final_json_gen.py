import pathlib, json, re

ROOT_RAW_PSRF = pathlib.Path("../psrd_data_modified").absolute().resolve()
ROOT_FINAL_PSRF = pathlib.Path("../psrd_final").absolute().resolve()


def do_items():

    all_items = []

    for fpath in ROOT_RAW_PSRF.joinpath("core_rulebook").joinpath("item").rglob( "*.json" ):
        f_json = json.loads( fpath.read_text()
                             .replace("&mdash;", "-")
                             .replace("&mdash", "-")
                             .replace("&ndash;", "-")
                             .replace("&times;", "x")
                             )

        f_json["name"] = " ".join( [i.strip() for i in reversed(f_json["name"].split(","))] )

        # Use the first line of the 'sections' section as the description if there is one
        if "body" not in f_json and "sections" in f_json and "body" in f_json["sections"][0]:
            f_json["body"] = f_json["sections"][0]["body"]

        # Get rid of html nonsense
        if "body" in f_json:
            f_json["body"] = re.sub( r"<[^<>]*>", "", f_json["body"] )
            f_json["description"] = f_json["body"]
            del( f_json["body"] )

        # Kill the info we definitely don't need
        for keyname in [ "sections", "url", "source", "type", "subtype" ]:
            if keyname in f_json:
                del( f_json[keyname] )

        # Normalize prices into copper pieces
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
                f_json["price"] = price_num
            else:
                print(price_str)
        if "price" not in f_json:
            f_json["price"] = 0

        # Normalize weight into lbs
        # Weight
        if "weight" in f_json:
            try:
                if f_json["weight"] == "-":
                    f_json["weight"] = "0 lbs."

                f_json["weight"] = int( re.search("^(\d+) lbs?", f_json["weight"]).group(1) )
            except:
                print( f_json["weight"] )
                raise
        else:
            f_json["weight"] = 0

        # Unpack auras into strength and type
        if "aura" in f_json:
            aura_str = f_json["aura"]
            if not aura_str.startswith( "no aura" ):
                aura_tokens = aura_str.split(" ")
                if len(aura_tokens) == 2:
                   f_json["aura_strength"], f_json["aura_value"] = aura_tokens

                elif len(aura_tokens) and aura_tokens[0] in ["strong", "faint", "moderate"]:
                    f_json["aura_strength"] = aura_tokens[0]
                    f_json["aura_value"] = "unknown"

            del(f_json["aura"])


        # work out what we are, hopefully

        if "misc" in f_json and "Weapon" in f_json["misc"]:
            item_type = "weapon"
        elif "misc" in f_json and "Armor" in f_json["misc"]:
            item_type = "armour"
        elif "slot" in f_json:
            if f_json["slot"] == "none":
                item_type = "misc"
            else:
                item_type = "equippable"
        elif "misc" in f_json and "null" in f_json["misc"] and "Gear Type" in f_json["misc"]["null"]:
            gt = f_json["misc"]["null"]["Gear Type"]
            if gt in ["Adventuring Gear", "Tools and Skill Kits", "Mounts and Related Gear"]:
                item_type = "gear"
            elif gt in ["Special Substances and Items", "Food, Drink, and Lodging"]:
                item_type = "consumable"
            else:
                raise Exception
        else:
            raise Exception
        f_json["item_type"] = item_type



        # Deal with 'misc' section
        if "misc" in f_json:
            if "Construction" in f_json["misc"]:
                del( f_json["misc"]["Construction"] )
            if "null" in f_json["misc"]:
                del( f_json["misc"]["null"] )
            if "Weapon" in f_json["misc"]:
                f_json["weapon"] = f_json["misc"]["Weapon"]
                del(f_json["misc"]["Weapon"])
            if "Armor" in f_json["misc"]:
                f_json["armour"] = f_json["misc"]["Armor"]
                del(f_json["misc"]["Armor"])

            if not f_json["misc"]:
                del(f_json["misc"])
            else:
                print(f_json)

        # No blank slots please
        if "slot" in f_json and f_json["slot"] == "none":
            del(f_json["slot"])


        # Armor parsing
        if "armour" in f_json:
            for v in ["Armor Bonus", "Armor Check Penalty", "Maximum Dex Bonus", "Shield Bonus"]:
                if v in f_json["armour"]:
                    try:
                        f_json["armour"][v] = int( f_json["armour"][v] )
                    except:
                        f_json["armour"][v] = 0
            if "Arcane Spell Failure Chance" in f_json["armour"]:
                try:
                    f_json["armour"]["Arcane Spell Failure Chance"] = int(
                        re.search( r"^(\d+)%", f_json["armour"]["Arcane Spell Failure Chance"] ).group(1) )
                except:
                    print( f_json["name"], f_json["armour"])
                    raise

            if "Speed (20 ft.)" in f_json["armour"]:
                try:
                    f_json["armour"]["Speed 20"] = int( re.search(r"^(\d+)", f_json["armour"]["Speed (20 ft.)"] ).group(1) )
                except:
                    pass
                finally:
                    del( f_json["armour"]["Speed (20 ft.)"] )

            if "Speed (30 ft.)" in f_json["armour"]:
                try:
                    f_json["armour"]["Speed 30"] = int( re.search(r"^(\d+)", f_json["armour"]["Speed (30 ft.)"] ).group(1) )
                except:
                    pass
                finally:
                    del( f_json["armour"]["Speed (30 ft.)"] )

        # Weapon parsing

        if "weapon" in f_json:
            for v in ["Dmg (T)", "Dmg (S)", "Dmg (L)"]:
                if v in f_json["weapon"]:
                    del( f_json["weapon"][v] )
            if "Dmg (M)" in f_json["weapon"]:
                f_json["weapon"]["Damage"] = f_json["weapon"]["Dmg (M)"]
                del( f_json["weapon"]["Dmg (M)"] )
            if "Range" in f_json["weapon"]:
                try:
                    f_json["weapon"]["Range"] = int(
                        re.search( r"^(\d+)", f_json["weapon"]["Range"] ).group(1) )
                except:
                    print( f_json["name"], f_json["weapon"])
                    raise





        all_items.append( f_json )

    ROOT_FINAL_PSRF.joinpath( "items.json" ).write_text(
        json.dumps( all_items, indent=2, sort_keys=True )
    )


def do_creatures():

    all_creatures = []

    for fpath in ROOT_RAW_PSRF.joinpath("bestiary").joinpath("creature").rglob( "*.json" ):
        f_json = json.loads( fpath.read_text()
                             .replace("&mdash;", "-")
                             .replace("&mdash", "-")
                             .replace("&ndash;", "-")
                             .replace("&times;", "x")
                             )

        # Delete junk fields
        for keyname in [ "url", "organization", "racial_modifiers", "sections", "cr", "skills", "source", "environment", "type", "treasure"]:
            if keyname in f_json:
                del( f_json[keyname] )

        # Uncomma the names
        f_json["name"] = " ".join( [i.strip() for i in reversed(f_json["name"].split(","))] )

        # Intify the things

        for v in ["strength", "dexterity", "intelligence", "wisdom", "charisma", "xp"]:
            try:
                f_json[v] = int(f_json[v].replace(",", ""))
            except:
                f_json[v] = 0

        if "constitution" in f_json:
            try:
                f_json["constitution"] = int( f_json["constitution"])
            except:
                f_json["constitution"] = 10

        for v in ["will", "fortitude", "reflex", "init", "cmd", "cmb", "base_attack"]:
            try:
                f_json[v] = int( re.search( r"^([+-]?\d+)", f_json[v] ).group(1) )
            except:
                print(f_json["name"], f_json[v])
                raise

        # Reach

        if "reach" in f_json:
            try:
                f_json["reach"] = int(
                    re.search( r"^(\d+)", f_json["reach"] ).group(1) )
            except:
                print( f_json["name"], f_json["reach"])
                raise


        # Clean up spells

        if "spells" in f_json:

            spells = {}
            for i in f_json["spells"]:
                if len(i.keys()) == 1:
                    spells[ list(i.keys())[0] ] = re.sub( r"<[^<>]*>", "", i[ list(i.keys())[0] ])
            f_json["spells"] = spells

            if "spell-like abilities" in f_json["spells"]:
                f_json["spell_like_abilities"] = f_json["spells"]["spell-like abilities"]
                del ( f_json["spells"]["spell-like abilities"] )

            if "spells known" in f_json["spells"]:
                f_json["spells_known"] = f_json["spells"]["spells known"]
                del ( f_json["spells"]["spells known"] )

            if "spells prepared" in f_json["spells"]:
                f_json["spells_prepared"] = f_json["spells"]["spells prepared"]
                del ( f_json["spells"]["spells prepared"] )

            del( f_json["spells"] )


        all_creatures.append( f_json )

    ROOT_FINAL_PSRF.joinpath( "creatures.json" ).write_text(
        json.dumps( all_creatures, indent=2, sort_keys=True )
    )

def main():
    do_items()
    do_creatures()

if __name__ == "__main__":
    main()