return {

    -------------
    -- Weapons

    ["weapon.longsword"] = {
        description = "This sword is about 3-1/2 feet in length.",
        item_type = "weapon",
        name = "Longsword",
        price = 1500,
        slot = "weapon",
        sprite = "dawnlike/MedWep_001",
        weapon = {
            crit_lower = 19,
            crit_mult = 2,
            damage_dcount = 1,
            damage_dsize = 8,
            damage_type = "S",
            proficiency = "Martial",
            weapon_class = "One-Handed Melee"
        },
        weight = 4
    },
    ["weapon.dagger"] = {
        description = "A dagger has a blade that is about 1 foot in length.",
        item_type = "weapon",
        name = "Dagger",
        price = 200,
        slot = "weapon",
        sprite = "dawnlike/ShortWep_001",
        weapon = {
            Range = 10,
            crit_lower = 19,
            crit_mult = 2,
            damage_dcount = 1,
            damage_dsize = 4,
            damage_type = "P,S",
            proficiency = "Simple",
            weapon_class = "Light Melee"
        },
        weight = 1
    },

    ----------------
    -- Armour

    ["armour.banded_mail"] = {
        name = "Banded Mail",
        description = "Overlapping strips of metal, fastened to a leather backing.",
        item_type = "armour",
        price = 25000,
        sprite = "dawnlike/Armor_053",
        weight = 35,
        armour = {
            max_dex = -6,
            armour_bonus = 7,
            armour_check = -6,
            armour_type = "heavy",
            max_speed = 20,
            spell_failure = 35
        },
},


}

