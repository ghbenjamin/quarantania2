return {
  version = "1.5",
  luaversion = "5.1",
  tiledversion = "1.7.1",
  orientation = "orthogonal",
  renderorder = "right-down",
  width = 20,
  height = 20,
  tilewidth = 32,
  tileheight = 32,
  nextlayerid = 4,
  nextobjectid = 11,
  properties = {},
  tilesets = {
    {
      name = "dawnlike",
      firstgid = 1,
      filename = "../../tools/maps/tilesets/dawnlike.tsx"
    }
  },
  layers = {
    {
      type = "tilelayer",
      x = 0,
      y = 0,
      width = 20,
      height = 20,
      id = 1,
      name = "Floor",
      visible = true,
      opacity = 1,
      offsetx = 0,
      offsety = 0,
      parallaxx = 1,
      parallaxy = 1,
      properties = {},
      encoding = "base64",
      compression = "zlib",
      data = "eJx7w8TA8GYUj+JRPIpH8SgmEQMASyNz8A=="
    },
    {
      type = "tilelayer",
      x = 0,
      y = 0,
      width = 20,
      height = 20,
      id = 2,
      name = "Walls",
      visible = true,
      opacity = 1,
      offsetx = 0,
      offsety = 0,
      parallaxx = 1,
      parallaxy = 1,
      properties = {},
      encoding = "base64",
      compression = "zlib",
      data = "eJxjYBgFo2AUjIJRMApIBwAGQAAB"
    },
    {
      type = "objectgroup",
      draworder = "topdown",
      id = 3,
      name = "Spawns",
      visible = true,
      opacity = 1,
      offsetx = 0,
      offsety = 0,
      parallaxx = 1,
      parallaxy = 1,
      properties = {},
      objects = {
        {
          id = 1,
          name = "player-spawn",
          type = "",
          shape = "point",
          x = 307.033,
          y = 272.007,
          width = 0,
          height = 0,
          rotation = 0,
          visible = true,
          properties = {}
        },
        {
          id = 2,
          name = "player-spawn",
          type = "",
          shape = "point",
          x = 362.925,
          y = 307.778,
          width = 0,
          height = 0,
          rotation = 0,
          visible = true,
          properties = {}
        },
        {
          id = 5,
          name = "enemy-spawn",
          type = "",
          shape = "point",
          x = 128.179,
          y = 144.574,
          width = 0,
          height = 0,
          rotation = 0,
          visible = true,
          properties = {
            ["name"] = "Poison Frog"
          }
        },
        {
          id = 8,
          name = "enemy-spawn",
          type = "",
          shape = "point",
          x = 524.639,
          y = 495.575,
          width = 0,
          height = 0,
          rotation = 0,
          visible = true,
          properties = {
            ["name"] = "Poison Frog"
          }
        },
        {
          id = 9,
          name = "enemy-spawn",
          type = "",
          shape = "point",
          x = 555.939,
          y = 147.555,
          width = 0,
          height = 0,
          rotation = 0,
          visible = true,
          properties = {
            ["name"] = "Poison Frog"
          }
        },
        {
          id = 10,
          name = "enemy-spawn",
          type = "",
          shape = "point",
          x = 206.428,
          y = 501.537,
          width = 0,
          height = 0,
          rotation = 0,
          visible = true,
          properties = {
            ["name"] = "Poison Frog"
          }
        }
      }
    }
  }
}
