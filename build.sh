#!/bin/bash

# used RADDebugger build script as reference- https://github.com/EpicGamesExt/raddebugger

# ----- args
for arg in "$@";        do declare $arg='1'; done
if [ ! -v debug ];      then release=1; fi

# ----- defines
gcc_include="-I./src/"
if [ -v test ]; then gcc_include+=" -I./test"; fi

gcc_flags="-std=gnu11 -Wall -Wextra -Werror -Wshadow -Wpedantic -Wnull-dereference -Wunused -Wconversion -Wno-pointer-sign"
gcc_compile="gcc -g -O2 ${gcc_include} ${gcc_flags}"
gcc_debug="gcc -g -O0 ${gcc_include} ${gcc_flags}"
gcc_link="-lpthread -lm -lrt -ldl"

if [ -v debug ]; then gcc_compile=${gcc_debug}; fi

# ----- src files
main_file="./src/main.c"
html_files="./src/html/parser.c"
util_files="./src/util/utf8.c"

src_files="${html_files} ${util_files}"

# ----- test files
test_main_file="./test/test_main.c ./test/test_utils.c"
test_util_files="./test/util/test_utf8.c"

test_files="${test_util_files}"

# ----- build
mkdir -p out
rm -rf ./out/*

if [ -v test ]; then
    files="${test_main_file} ${src_files} ${test_files}";
    ${gcc_compile} ${files} ${gcc_link} -o ./out/test_vl;
else
    files="${main_file} ${src_files}";
    ${gcc_compile} ${files} ${gcc_link} -o ./out/vl
fi
