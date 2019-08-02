cmake_minimum_required(VERSION 3.10)

project(sasm DESCRIPTION "sasm intepreter")
set(sasm_VERSION_MAJOR 0)
set(sasm_VERSION_MINOR 1)
include(GNUInstallDirs)

set(CMAKE_C_STANDARD 99)

add_subdirectory(libsasm)
add_subdirectory(sasm)