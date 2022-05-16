return {

    -- Movement actions

    ["short-step"] = {
        name = "Short Step",
        type = "move",
        speed = 1,
        provokes = false,
        description = "Take a single step in any direction. Does not provoke attacks of opportunity.",
        icon = "game_ui/w-24-walking-boot"
    },

    ["move"] = {
        name = "Move",
        type = "move",
        speed = 1,
        provokes = true,
        description = "Walk to a new position.",
        icon = "game_ui/w-24-walk"
    },


    -- Attack Actions


    ["strike"] = {
        name = "Strike",
        type = "attack",
        speed = 1,
        provokes = false,
        description = "Strike a single enemy with your primary melee weapon.",
        icon = "game_ui/w-24-sword-brandish"
    },

    ["power-attack"] = {
        name = "Power Attack",
        type = "attack",
        speed = 1,
        provokes = false,
        description = "A melee strike which sacrifices accuracy for damage.",
        icon = "game_ui/w-24-axe-swing"
    }
}