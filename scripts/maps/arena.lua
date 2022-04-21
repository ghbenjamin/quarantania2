return {
  version = "1.5",
  luaversion = "5.1",
  tiledversion = "1.7.1",
  orientation = "orthogonal",
  renderorder = "right-down",
  width = 32,
  height = 21,
  tilewidth = 32,
  tileheight = 32,
  nextlayerid = 6,
  nextobjectid = 21,
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
      width = 32,
      height = 21,
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
      data = "eJwTZWZgEB3Fo3gUj+JRPIpH8SgexaOYThgAVAU/AQ=="
    },
    {
      type = "tilelayer",
      x = 0,
      y = 0,
      width = 32,
      height = 21,
      id = 3,
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
      data = "eJxjYBh4kMjJwJBEAKcSoYYUnMyJsL+AE7fbSFFDCigYtX/U/mFmPyluHLV/1P6hZH/hANtPqpmj9g8d++up1KZpJEFtA5XrE3IBAA6LNN0="
    },
    {
      type = "objectgroup",
      draworder = "topdown",
      id = 5,
      name = "Decor",
      visible = true,
      opacity = 1,
      offsetx = 0,
      offsety = 0,
      parallaxx = 1,
      parallaxy = 1,
      properties = {},
      objects = {
        {
          id = 18,
          name = "cobweb",
          type = "",
          shape = "rectangle",
          x = 288,
          y = 64,
          width = 32,
          height = 32,
          rotation = 0,
          gid = 455,
          visible = true,
          properties = {}
        },
        {
          id = 19,
          name = "cobweb",
          type = "",
          shape = "rectangle",
          x = 576,
          y = 576,
          width = 32,
          height = 32,
          rotation = 0,
          gid = 457,
          visible = true,
          properties = {}
        },
        {
          id = 20,
          name = "shelf",
          type = "",
          shape = "rectangle",
          x = 928,
          y = 64,
          width = 32,
          height = 32,
          rotation = 0,
          gid = 357,
          visible = true,
          properties = {}
        }
      }
    },
    {
      type = "objectgroup",
      draworder = "topdown",
      id = 4,
      name = "Fixed",
      visible = true,
      opacity = 1,
      offsetx = 0,
      offsety = 0,
      parallaxx = 1,
      parallaxy = 1,
      properties = {},
      objects = {
        {
          id = 14,
          name = "torch",
          type = "",
          shape = "rectangle",
          x = 864,
          y = 512,
          width = 32,
          height = 32,
          rotation = 0,
          gid = 385,
          visible = true,
          properties = {
            ["light-colour"] = "#fffdffdc",
            ["light-intensity"] = 1
          }
        },
        {
          id = 15,
          name = "torch",
          type = "",
          shape = "rectangle",
          x = 864,
          y = 128,
          width = 32,
          height = 32,
          rotation = 0,
          gid = 385,
          visible = true,
          properties = {
            ["light-colour"] = "#fffdffdc",
            ["light-intensity"] = 1
          }
        },
        {
          id = 16,
          name = "torch",
          type = "",
          shape = "rectangle",
          x = 128,
          y = 512,
          width = 32,
          height = 32,
          rotation = 0,
          gid = 385,
          visible = true,
          properties = {
            ["light-colour"] = "#fffdffdc",
            ["light-intensity"] = 1
          }
        },
        {
          id = 17,
          name = "torch",
          type = "",
          shape = "rectangle",
          x = 128,
          y = 128,
          width = 32,
          height = 32,
          rotation = 0,
          gid = 385,
          visible = true,
          properties = {
            ["light-colour"] = "#fffdffdc",
            ["light-intensity"] = 1
          }
        }
      }
    },
    {
      type = "objectgroup",
      draworder = "topdown",
      id = 2,
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
          x = 720.677,
          y = 464.609,
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
          x = 687.649,
          y = 499.732,
          width = 0,
          height = 0,
          rotation = 0,
          visible = true,
          properties = {}
        },
        {
          id = 3,
          name = "enemy-spawn",
          type = "",
          shape = "point",
          x = 681.357,
          y = 460.376,
          width = 0,
          height = 0,
          rotation = 0,
          visible = true,
          properties = {
            ["name"] = "Poison Frog"
          }
        },
        {
          id = 4,
          name = "enemy-spawn",
          type = "",
          shape = "point",
          x = 746.424,
          y = 494.751,
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
