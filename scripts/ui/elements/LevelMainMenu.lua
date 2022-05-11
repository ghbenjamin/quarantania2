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
        type = "button",
        id = "level-menu-resume",
        text = "Resume",
        text_size = 18
     },
     {
         layout = { type = "center" },
         preferred_size = { width = 100, height = 25 },
         type = "button",
         id = "level-menu-options",
         text = "Options",
         text_size = 18
     },
     {
         layout = { type = "center" },
         preferred_size = { width = 100, height = 25 },
         type = "button",
         id = "level-menu-main",
         text = "Main Menu",
         text_size = 18
     },
     {
         layout = { type = "center" },
         preferred_size = { width = 100, height = 25 },
         type = "button",
         id = "level-menu-quit",
         text = "Quit",
         text_size = 18
     }
 }
}