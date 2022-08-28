return {

 padding = 8,
 layout = {
    type = "vertical",
    halign = "fill",
    spacing = 10
 },
 background = {
    type = "ninepatch",
    name = "simple-border-solid"
 },
 children = {
     {
        layout = { type = "center" },
        preferred_size = { width = 100, height = 25 },
        type = "label",
        id = "modifier-overlay-title",
        text = "Modifiers",
        text_size = 18,
        text_colour = "#444444"
     },
     {
        type = "horizontal_rule",
        colour = "Grey"
     },
     {
        id = "modifier-list-container",
        layout = {
            type = "vertical",
            halign = "fill",
            spacing = 10
         },
     }
 }
}