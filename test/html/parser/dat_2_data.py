import os
import json
import argparse

DATA = "#data"
ERRORS = "#errors"
NEW_ERRORS = "#new-errors"
DOCUMENT = "#document"
FRAGMENT = "#document-fragment"
headers = { DATA, ERRORS, NEW_ERRORS, DOCUMENT, FRAGMENT }

def write_line(file, string):
    string = string.replace("\n", "\\n")
    file.write(string + '\n')


def write_lines(file, lines):
    for line in lines:
        write_line(file, line)


def parse_test(lines):
    test = {}
    key = None

    for line in lines:
        if line in headers:
            key = line
            test[key] = []
        else:
            test[key].append(line)

    if len(test[DATA]) > 1:
        test_line = test[DATA][0]
        for i in range(1, len(test[DATA])):
            test_line += "\\n"
            test_line += test[DATA][i]
        test[DATA] = [ test_line ]

    document_lines = []
    for i in range(len(test[DOCUMENT])):
        line = test[DOCUMENT][i]
        if not line.startswith("| "):
            document_lines[-1] = document_lines[-1] + "\\n" + line
        else:
            document_lines.append(line)
    test[DOCUMENT] = document_lines
    return test


def export_test(f_out, test):
    write_line(f_out, DATA)
    write_lines(f_out, test[DATA])

    write_line(f_out, ERRORS)
    write_lines(f_out, test.get(ERRORS, []))

    if test.get(NEW_ERRORS, []):
        write_line(f_out, NEW_ERRORS)
        write_lines(f_out, test.get(NEW_ERRORS, []))

    if (test.get(FRAGMENT, [])):
        write_line(f_out, FRAGMENT)
        write_lines(f_out, test.get(FRAGMENT, []))

    write_line(f_out, DOCUMENT)
    write_lines(f_out, test.get(DOCUMENT, []))


def parse_and_export(f_out, test_lines, not_supported, replacements, prepend_newline = False):
    test = parse_test(test_lines)
    assert(len(test[DATA]) != 0)

    test_data = test[DATA][0]

    if test_data in not_supported:  return
    if test.get(FRAGMENT, []):      return
    if "<math" in test_data:        return
    if "<svg" in test_data:         return

    if replacements.get(test_data, None):
        test = replacements.get(test_data, {})

    if prepend_newline: f_out.write("\n")
    export_test(f_out, test)


def dat_2_data(args):
    i_path = args['i'] or './input'
    o_path = args['o'] or './data'
    not_supported = set([])
    replacements = {}

    with open(os.path.join(i_path, '..', 'not_supported.json'), 'r') as f:
        data = json.load(f)
        not_supported = set(data)

    with open(os.path.join(i_path, '..', 'replacements.json'), 'r') as f:
        replacements = json.load(f)

    for root, dirs, files in os.walk(i_path, topdown=False):
        for file in files:
            file_name = os.path.join(root, file)
            with open(file_name, 'r') as f_in:
                file_name_o = os.path.join(o_path, file.replace('.dat', '.data'))
                with open(file_name_o, 'w', encoding='utf-8') as f_out:
                    all_lines = f_in.readlines()
                    test_lines = []
                    i = 0
                    prepend = False

                    while i < len(all_lines):
                        line = all_lines[i].strip("\n")

                        if (line == DATA and len(test_lines) > 0):

                            test_lines.pop()
                            parse_and_export(f_out, test_lines, not_supported, replacements, prepend)

                            if f_out.tell() > 0: prepend = True

                            test_lines = [line]
                        else:
                            test_lines.append(line)

                        i += 1

                    # last test
                    parse_and_export(f_out, test_lines, not_supported, replacements, prepend)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Script that converts json files from html5-lib to data files')
    parser.add_argument('-i', help='input folder')
    parser.add_argument('-o', help='output folder')
    args = vars(parser.parse_args())
    dat_2_data(args)
    