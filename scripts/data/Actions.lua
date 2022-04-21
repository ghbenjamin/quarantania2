return {
    ["short-step"] = {
        name = "Short Step",
        id = "short-step",
        type = "move",
        speed = "Move",
        provokes = false,
        description = "Take a single step in any direction. Does not provoke attacks of opportunity.",
        icon = "game_ui/w-24-walking-boot"
    },
    ["move"] = {
        name = "Move",
        id = "move",
        type = "move",
        speed = "Move",
        provokes = true,
        description = "Walk to a new position.",
        icon = "game_ui/w-24-walk"
    },
    ["strike"] = {
        name = "Strike",
        id = "strike",
        type = "attack",
        speed = "Standard",
        provokes = false,
        description = "Strike a single enemy with your primary melee weapon.",
        icon = "game_ui/w-24-sword-brandish"
    },
    ["full-attack"] = {
        name = "Full Attack",
        id = "full-attack",
        type = "attack",
        speed = "FullRound",
        provokes = false,
        description = "Spend the turn attacking with all available weapons.",
        icon = "game_ui/w-24-sword-array"
    },
    ["power-attack"] = {
        name = "Power Attack",
        id = "power-attack",
        type = "attack",
        speed = "Standard",
        provokes = false,
        description = "A melee strike which sacrifices accuracy for damage.",
        icon = "game_ui/w-24-axe-swing"
    }
}