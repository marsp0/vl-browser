#!/bin/bash

# ----- args
for arg in "$@";        do declare $arg='1'; done
if [ ! -v debug ];      then release=1; fi

# ----- defines
gcc_include="-I../src/"
gcc_flags="-std=gnu11 -Wall -Wextra -Werror -Wshadow -Wpedantic -Wnull-dereference -Wunused -Wconversion -Wno-pointer-sign"
gcc_compile="gcc -g -O2 ${gcc_include} ${gcc_flags}"
gcc_debug="gcc -g -O0 ${gcc_include} ${gcc_flags}"
gcc_link="-lpthread -lm -lrt -ldl"

if [ -v debug ]; then gcc_compile=${gcc_debug}; fi

# ----- files
src_files="./src/main.c"

files+=${src_files}

# ----- build
mkdir -p out
${gcc_compile} ${files} ${gcc_link} -o ./out/vl
