from pathlib import Path
import sys, os, json
from PIL import Image

"""
Usage: 
    python tilesheet_stitcher.py my_dir
    
Where my_dir is the path to a system directory containing some number of .png files containing 
spritesheets which are to combined into one master spritesheet.

Outputs the combined .png file as well as a json file describing the contents. 
"""


# Pixel sizes of the tiles to be stitched together
INPUT_TILE_SIZE = 32

# Initial size of the image to create
# NB. this is just laziness, should really just do a two pass loop through the input tiles
# to work this out
OUTPUT_TILE_SIZE = 10, 64

ADD_DIGITS = False

def main():
    # Grab the input path from the system arguments - this is hopefully a directory described in a way
    # pathlib can understand
    target_dir = Path(sys.argv[1]).resolve()
    out_name = sys.argv[2]

    # Grab the paths of all the .png files in the target directory
    target_paths = [f for f in target_dir.glob("*.png") if f.is_file()]

    gid_list = []

    # Create a blank image to copy the tiles onto
    master_img = Image.new( "RGBA", (OUTPUT_TILE_SIZE[0] * INPUT_TILE_SIZE, OUTPUT_TILE_SIZE[1] * INPUT_TILE_SIZE))

    next_tile_coord = [0, 0]
    gid = 0
    last_stem_idx = 0
    last_stem = None

    for p in target_paths:
        image = Image.open(p)
        p_stem = p.stem

        # Massage the names of the files to play nicely with the Dawnlike tile naming convention
        if p_stem.endswith("1"):
            continue

        if p_stem.endswith("0"):
            p_stem = p_stem[:-1]

        img_px_size = image.size
        img_tile_size = tuple( i // INPUT_TILE_SIZE for i in img_px_size )

        # Walk over each of the tiles in the input file (assuming no margins or spacing)
        img_tile_size_w, img_tile_size_h = img_tile_size

        for j in range(img_tile_size_h):
            for i in range(img_tile_size_w):

                px_box = (i * INPUT_TILE_SIZE, j * INPUT_TILE_SIZE)
                px_box_region = image.crop((px_box[0], px_box[1], px_box[0] + INPUT_TILE_SIZE, px_box[1] + INPUT_TILE_SIZE))

                # If the number of unique colours in this tile is 1, then the tile is probably blank - ignore it
                if len(px_box_region.getcolors()) > 1:
                    master_target_px = (next_tile_coord[0] * INPUT_TILE_SIZE, next_tile_coord[1] * INPUT_TILE_SIZE, )
                    master_img.paste(px_box_region, master_target_px)

                    if p_stem == last_stem:
                        last_stem_idx += 1
                    else:
                        last_stem = p_stem
                        last_stem_idx = 1

                    if ADD_DIGITS:
                        gid_list.append([f"{p_stem}_{last_stem_idx:03}", gid])
                    else:
                        gid_list.append([f"{p_stem}", gid])
                    gid += 1

                    next_tile_coord[0] += 1
                    if next_tile_coord[0] == OUTPUT_TILE_SIZE[0]:
                        next_tile_coord[1] += 1
                        next_tile_coord[0] = 0

    # Crop our image to the actual size required to store our tiles
    row_count = gid // OUTPUT_TILE_SIZE[0] + 1
    master_img = master_img.crop((0, 0, OUTPUT_TILE_SIZE[0] * INPUT_TILE_SIZE, row_count * INPUT_TILE_SIZE))

    # Construct a json object in the format that Quarantania will expect to describe the above tiles
    json_obj = {}
    json_obj['meta'] = {
        "type": "tiled",
        "margin": 0,
    }
    json_obj['data'] = { k: v for k, v in gid_list }

    # Dump our image and json files to disk
    with open( f"{out_name}.json", "w" ) as outfile:
        json.dump(json_obj, outfile, indent=4, sort_keys=True)

    master_img.save(f"{out_name}.png")

if __name__ == "__main__":
    main()