
import json, csv, pathlib

if __name__ == "__main__":

    tilesheet_path = pathlib.Path( r"C:\Users\brh\Docs\Programming\C++\quarantania2\resource\spritesheet\dawnlike_items.json" )
    tilesheet_obj = json.loads( tilesheet_path.read_text() )

    with open("../item_gids.csv") as f:
        reader = csv.reader(f)
        lines = list(reader)

    reverse_map = {}
    for k, v in tilesheet_obj["data"].items():
        reverse_map[v] = k

    out_map = {}
    for l in lines:
        name, _, gid = l
        if gid:
            out_map[name] = {}
            out_map[name]["sheet"] = "dawnlike_items"
            out_map[name]["sprite"] = reverse_map[int(gid)]

    pathlib.Path("items_icons.json").resolve().write_text( json.dumps( out_map, indent=2 ) )


