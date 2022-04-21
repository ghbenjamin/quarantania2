return {
    ["weapon.longsword"] = {
        description = "This sword is about 3-1/2 feet in length.",
        item_type = "weapon",
        name = "Longsword",
        price = 1500,
        slot = "weapon",
        sprite_name = "MedWep_001",
        sprite_sheet = "dawnlike",
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
        sprite_name = "ShortWep_001",
        sprite_sheet = "dawnlike",
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
    }
}

