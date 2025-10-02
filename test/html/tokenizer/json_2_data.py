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
    file.write(' '.join([get_val(x) for x in arr]) + '\n')

def json_2_data(args):
    i_path = args['i']
    o_path = args['o']
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

                        write_line(f_out, '#output')
                        for token in test.get('output', []):
                            t_type = token[0]
                            if (t_type == 'DOCTYPE'):
                                name = token[1] or ''
                                public = token[2] or ''
                                system = token[3] or ''
                                line = f'{token[0]} "{name}" "{public}" "{system}" {get_val(token[4])}'
                                write_line(f_out, line)

                            elif (t_type == "Character"):
                                for char in token[1]:
                                    write_line(f_out, f'{token[0]} {char}')

                            elif (t_type == "Comment"):
                                write_array(f_out, token)

                            elif (t_type == "EndTag"):
                                line = f'{token[0]} "{token[1]}"'
                                write_line(f_out, line)

                            elif (t_type == "StartTag"):
                                has_bool = len(token) == 4
                                line = f'{token[0]} "{token[1]}"'

                                if has_bool: 
                                    line += f' {get_val(token[3])}'
                                write_line(f_out, line)

                                for key in token[2].keys():
                                    name = key.replace('"', '\\"')
                                    write_line(f_out, f'Attr "{name}" "{token[2][key]}"')

                        if i < (len(tests) - 1):
                            f_out.write("\n")
                

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Script that converts json files from html5-lib to data files')
    parser.add_argument('-i', help='input folder', required=True)
    parser.add_argument('-o', help='output folder', required=True)
    args = vars(parser.parse_args())
    json_2_data(args)
    