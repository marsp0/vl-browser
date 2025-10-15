import os
import json
import argparse

def get_val(x):
    if (type(x) == bool):
        return 'true' if x else 'false'
    elif (x == None):
        return 'null'
    else:
        return x

def write_line(file, string):
    string = string.replace("\n", "\\n")
    file.write(string + '\n')

def write_array(file, arr):
    arr = ' '.join([get_val(x) for x in arr])
    arr = arr.replace('\n', '\\n')
    file.write(arr + '\n')

def json_2_data(args):
    i_path = args['i'] or './input'
    o_path = args['o'] or './data'
    not_supported = set([])

    with open(os.path.join(i_path, '..', 'not_supported.json'), 'r') as f:
        data = json.load(f)
        not_supported = set(data)

    for root, dirs, files in os.walk(i_path, topdown=False):
        for file in files:
            file_name = os.path.join(root, file)
            with open(file_name, 'r') as f_in:
                file_name_o = os.path.join(o_path, file.replace('.json', '.data'))
                with open(file_name_o, 'w', encoding='utf-8') as f_out:
                    data = json.load(f_in)
                    tests = data.get('tests', [])
                    for i in range(len(tests)):
                        test = tests[i]
                        description = test.get('description', '')

                        if description in not_supported: continue

                        write_line(f_out, '#description')
                        write_line(f_out, description)

                        test_data = test.get('input', '')
                        write_line(f_out, '#data')
                        write_line(f_out, f'{test_data}')

                        write_line(f_out, '#errors')
                        for error in test.get('errors', []):
                            code = error.get('code')
                            line = error.get('line')
                            col = error.get('col')
                            write_line(f_out, f"({line}, {col}): {code}")

                        write_line(f_out, '#states')
                        states = test.get('initialStates', ["Data state"])
                        for state in states:
                            write_line(f_out, f'{state}')

                        # write_line(f_out, '#output')
                        for token in test.get('output', []):
                            t_type = token[0]
                            if (t_type == 'DOCTYPE'):

                                name = token[1] or ''
                                public = token[2] or ''
                                system = token[3] or ''

                                write_line(f_out, "#doctype")
                                write_line(f_out, name)

                                write_line(f_out, "#doctype-quirks")
                                write_line(f_out, get_val(token[4]))

                                if (public):
                                    write_line(f_out, "#doctype-public")
                                    write_line(f_out, public)
                                if (system):
                                    write_line(f_out, "#doctype-system")
                                    write_line(f_out, system)

                            elif (t_type == "Character"):
                                for char in token[1]:
                                    write_line(f_out, "#character")
                                    write_line(f_out, char)

                            elif (t_type == "Comment"):
                                write_line(f_out, "#comment")
                                write_line(f_out, token[1])

                            elif (t_type == "EndTag"):
                                write_line(f_out, "#end-tag")
                                write_line(f_out, token[1])

                            elif (t_type == "StartTag"):
                                has_bool = len(token) == 4

                                write_line(f_out, "#start-tag")
                                write_line(f_out, token[1])

                                if has_bool: 
                                    write_line(f_out, "#start-tag-self-close")
                                    write_line(f_out, get_val(token[3]))

                                for key in token[2].keys():
                                    val = token[2][key]
                                    write_line(f_out, "#attr-name")
                                    write_line(f_out, key)
                                    write_line(f_out, "#attr-value")
                                    write_line(f_out, val)

                        write_line(f_out, "#end-test")
                        if i < (len(tests) - 1):
                            f_out.write("\n")


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Script that converts json files from html5-lib to data files')
    parser.add_argument('-i', help='input folder')
    parser.add_argument('-o', help='output folder')
    args = vars(parser.parse_args())
    json_2_data(args)
    