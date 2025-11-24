import os
import json
import argparse


class Context:
    
    def __init__(self):
        self.f_name = ""
        self.f_out = None

    def reload(self, path, f_name):
        self.f_name = f_name
        if (self.f_out):
            self.f_out.close()

        self.f_out = open(os.path.join(path, f_name + ".txt"), "w")

    def write_line(self, string):
        string = string.replace("\n", "\\n")
        self.f_out.write(string + '\n')

    def export_test(self, test, tokens):
        self.write_line("#data")
        self.write_line(test["data"])

        for i in range(len(tokens)):
            t = tokens[i]
            self.write_line("#token-type")
            self.write_line(t["type"])
            if not t["structured"]:
                continue
            self.write_line("#token-value")
            self.write_line(str(t["structured"]["value"]))

        self.write_line("#end-test")
        
    def __del__(self):
        if (self.f_out):
            self.f_out.close()


def translate(args, ctx):
    i_path = args['i'] or './input'
    o_path = args['o'] or './data'

    for (root, _, files) in os.walk(i_path, topdown=True):
        if ("source.css" not in files) or ("tokens.json" not in files):
            continue

        test = {}
        f_source = os.path.join(root, "source.css")
        f_tokens = os.path.join(root, "tokens.json")

        with open(f_source, "r") as f:
            test["data"] = f.read()

        tokens = {}
        with open(f_tokens, "r") as f:
            tokens = json.load(f)

        f_name = root.split("/")[-2]

        if (ctx.f_name != f_name):
            ctx.reload(o_path, f_name)

        ctx.export_test(test, tokens)

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Script that converts json files from html5-lib to data files')
    parser.add_argument('-i', help='input folder')
    parser.add_argument('-o', help='output folder')
    args = vars(parser.parse_args())
    translate(args, Context())
    