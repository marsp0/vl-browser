#!/bin/bash

# used RADDebugger build script as reference- https://github.com/EpicGamesExt/raddebugger

# ----- args
for arg in "$@";        do declare $arg='1'; done
if [ ! -v debug ];      then release=1; fi

# ----- defines
gcc_include="-I./src/"
gcc_flags="-std=gnu11 -Wall -Wextra -Werror -Wshadow -Wpedantic -Wnull-dereference -Wunused -Wconversion -Wno-pointer-sign"
gcc_compile="gcc -O2 ${gcc_include} ${gcc_flags}"
gcc_debug="gcc -g -O0 -fprofile-arcs -ftest-coverage ${gcc_include} ${gcc_flags}"
gcc_link="-lpthread -lm -lrt -ldl"

if [ -v debug ]; then gcc_compile=${gcc_debug}; fi

# ----- src files
main_file="./src/main.c"

html_files="./src/html/tokenizer.c ./src/html/parser.c"

dom_files="./src/dom/node.c ./src/dom/comment.c ./src/dom/document.c ./src/dom/doctype.c ./src/dom/text.c ./src/dom/element.c"
dom_files+=" ./src/dom/option.c ./src/dom/select.c"

util_files="./src/util/utf8.c"

core_files=" "

src_files="${html_files} ${util_files} ${core_files} ${dom_files}"

# ----- test files
test_main_file="./test/test_main.c ./test/test_utils.c"

test_util_files="./test/util/test_utf8.c"

#test_html_files=" ./test/html/test_tokenizer_test1.c ./test/html/test_tokenizer_test2.c ./test/html/test_tokenizer_test3.c ./test/html/test_tokenizer_rcdata.c "
#test_html_files+=" ./test/html/test_tokenizer_rawtext.c ./test/html/test_tokenizer_script_data.c"
test_html_files+=" ./test/html/test_parser.c ./test/html/test_parser_utils.c"

test_dom_files=" ./test/dom/test_node.c ./test/dom/test_node_utils.c"

test_core_files=" "

test_files="${test_util_files} ${test_dom_files} ${test_html_files} ${test_core_files}"

# ----- build
mkdir -p out
rm -rf ./out/*

echo "Compile tests"
files="${test_main_file} ${src_files} ${test_files}";
${gcc_compile} -I./test ${files} ${gcc_link} -o ./out/test_vl;

if [ $? -eq 0 ]; then
    echo "Compile browser"
    files="${main_file} ${src_files}";
    ${gcc_compile} ${files} ${gcc_link} -o ./out/vl;
fi
