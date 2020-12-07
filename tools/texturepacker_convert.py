import sys, pathlib, json

def do_conversion( in_path, out_path ):

    json_data_in = json.loads( in_path.read_text() )
    json_data_out = {
        "meta": {
            "type": "free"
        },
        "data": {

        }
    }

    for frame in json_data_in["frames"]:
        filename = pathlib.Path(frame["filename"]).stem
        json_data_out["data"][filename] = [
            frame["frame"]["x"],
            frame["frame"]["y"],
            frame["frame"]["w"],
            frame["frame"]["h"],
        ]

    out_path.write_text( json.dumps(json_data_out, indent=4) )

def main():
    in_path = pathlib.Path(sys.argv[1])
    out_path = pathlib.Path(sys.argv[2])

    do_conversion(in_path, out_path)

if __name__ == "__main__":
    main()