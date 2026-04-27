#!/bin/bash

files="src/main.cpp src/read_input.cpp src/HashTable/hash_table.cpp src/HashTable/hash_funcs.cpp src/List/my_list.cpp"

WARN_FLAGS="-std=c++17 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations \
    -Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts \
    -Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal \
    -Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline \
    -Wlogical-op -Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked \
    -Wpointer-arith -Winit-self -Wredundant-decls -Wshadow -Wsign-conversion -Wsign-promo \
    -Wstrict-null-sentinel -Wstrict-overflow=2 -Wsuggest-attribute=noreturn \
    -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override -Wswitch-default \
    -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast \
    -Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing \
    -Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation \
    -fstack-protector -fstrict-overflow -flto-odr-type-merging -pie -fPIE"

g++ -I./include/HashTable -I./include/List -I./include $files -O2 -DNDEBUG -ggdb3 -fno-omit-frame-pointer -march=native $WARN_FLAGS -o hash_table_program_after_2_aligned