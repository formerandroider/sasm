#cmake_minimum_required(VERSION 3.14)
project(SimpleASM C)

set(CMAKE_C_STANDARD 99)

add_executable(SimpleASM simpleasm.c simpleasm.h simpleasm_parser.tab.c lex.yy.c)