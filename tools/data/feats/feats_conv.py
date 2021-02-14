import sys, csv, json, pathlib
from typing import List, AnyStr


def name_to_id(name):
    return name.lower().replace(" ", "_").replace("(", "").replace(")", "")


def main():

    rows = []
    out = []

    with open("feats.csv", "r") as csvfile:
        reader = csv.reader(csvfile, )
        next(reader)
        for row in reader:
            rows.append(row)


    for row in rows:
        f_name, f_type, f_description, f_prerequisites, f_prerequisite_feats, f_benefit, f_normal, f_special, f_source = row

        json_data = {}
        json_data["id"] = name_to_id(f_name)
        json_data["name"] = f_name
        json_data["desc"] = f_description
        json_data["benefit"] = f_benefit

        if f_prerequisites:
            json_data["prereq"] = f_prerequisites
        if f_prerequisite_feats:
            json_data["prereq_feats"] = f_prerequisite_feats


        out.append(json_data)

    for feat in out:
        if "prereq_feats" in feat:
            clean = []
            for p in feat["prereq_feats"].split(","):
                clean.append( name_to_id(p.strip()) )
            feat["prereq_feats"] = clean




    out_path = pathlib.Path("feats.json")
    out_path.write_text( json.dumps(out, indent=2, sort_keys=True) )

if __name__ == "__main__":
    main()

