cmake_minimum_required(VERSION 3.10)

project(sasm DESCRIPTION "sasm intepreter")
set(sasm_VERSION_MAJOR 0)
set(sasm_VERSION_MINOR 1)
include(GNUInstallDirs)

set(CMAKE_C_STANDARD 99)

find_package(FLEX)
flex_target(sasm_lexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c)

find_package(BISON)
bison_target(sasm_parser simpleasm_parser.y ${CMAKE_CURRENT_BINARY_DIR}/simpleasm_parser.tab.c)

ADD_FLEX_BISON_DEPENDENCY(sasm_lexer sasm_parser)

configure_file(sasmlib.pc.in sasmlib.pc @ONLY)
configure_file (
        "${PROJECT_SOURCE_DIR}/sasm_config.h.in"
        "${PROJECT_BINARY_DIR}/sasm_config.h"
)

include_directories(. ${CMAKE_CURRENT_BINARY_DIR})

add_library(sasm SHARED simpleasm.c ${BISON_sasm_parser_OUTPUTS} ${FLEX_sasm_lexer_OUTPUTS} sasm_config.h)
set_target_properties(sasm PROPERTIES VERSION 1.0.0 SOVERSION 1 PUBLIC_HEADER include/sasm.h)
target_include_directories(sasm PRIVATE .)

install(TARGETS sasm
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/sasmlib.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

add_library(sasm_static STATIC simpleasm.c ${BISON_sasm_parser_OUTPUTS} ${FLEX_sasm_lexer_OUTPUTS} sasm_config.h)

add_executable(sasm_run sasm.c)
target_include_directories(sasm_run PRIVATE include)
target_link_libraries(sasm_run sasm_static)
