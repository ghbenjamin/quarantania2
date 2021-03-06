

from pathlib import Path
import sys, os, multiprocessing, subprocess


SVG_DIR = Path(r"C:\Users\brh\Pictures\game-icons-net\white-transparent")
OUT_DIR = Path(r"C:\Users\brh\Pictures\game-icons-net\white_transparent_png")

def main():

    # width = 24
    # height = 24
    # prefix = "w-24-"

    width = 32
    height = 32
    prefix = "w-"

    commands = []

    if not OUT_DIR.exists():
        OUT_DIR.mkdir()

    for f in SVG_DIR.glob("*.svg"):
        outpath = (OUT_DIR / (prefix+ f.stem)).with_suffix(".png")

        if not "backpack" in str(outpath):
            continue

        command = f"inkscape.com -w {width} -h {height} { str(f) } -e {outpath}"

        commands.append(command)


    for s in commands:
        subprocess.run(s)


if __name__ == "__main__":
    main()