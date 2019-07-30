cmake_minimum_required(VERSION 2.4)
cmake_policy(SET CMP0042 NEW)
macro(use_c99)
    if (CMAKE_VERSION VERSION_LESS "3.1")
        if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
            set (CMAKE_C_FLAGS "-std=gnu99 ${CMAKE_C_FLAGS}")
        endif ()
    else ()
        set (CMAKE_C_STANDARD 99)
    endif ()
endmacro(use_c99)

project(sasm DESCRIPTION "sasm intepreter")
set(sasm_VERSION_MAJOR 0)
set(sasm_VERSION_MINOR 1)
include(GNUInstallDirs)

use_c99()

add_library(sasm SHARED simpleasm.c simpleasm_parser.tab.c lex.yy.c sasm_config.h)
add_library(sasm_static STATIC simpleasm.c simpleasm_parser.tab.c lex.yy.c sasm_config.h)
set_target_properties(sasm PROPERTIES VERSION 1.0.0 SOVERSION 1 PUBLIC_HEADER include/sasm.h)
configure_file(sasmlib.pc.in sasmlib.pc @ONLY)
configure_file (
        "${PROJECT_SOURCE_DIR}/sasm_config.h.in"
        "${PROJECT_BINARY_DIR}/sasm_config.h"
)
target_include_directories(sasm PRIVATE .)

install(TARGETS sasm
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/sasmlib.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

add_executable(sasm_run sasm.c)
target_include_directories(sasm_run PRIVATE include)
target_link_libraries(sasm_run sasm_static)
