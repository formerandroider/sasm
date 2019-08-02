configure_file(sasmlib.pc.in sasmlib.pc @ONLY)
configure_file (
        "sasm_config.h.in"
        "${PROJECT_BINARY_DIR}/sasm_config.h"
)

include_directories(.)

find_package(FLEX)
flex_target(sasm_lexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c)

find_package(BISON)
bison_target(sasm_parser simpleasm_parser.y ${CMAKE_CURRENT_BINARY_DIR}/simpleasm_parser.tab.c)

ADD_FLEX_BISON_DEPENDENCY(sasm_lexer sasm_parser)
add_library(libsasm SHARED simpleasm.c simpleasm.h ${BISON_sasm_parser_OUTPUTS} ${FLEX_sasm_lexer_OUTPUTS} sasm_config.h)
set_target_properties(libsasm PROPERTIES VERSION 1.0.0 SOVERSION 1 PUBLIC_HEADER include/sasm.h)
target_include_directories(libsasm PRIVATE ../include)

install(TARGETS libsasm
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_BINARY_DIR}/sasmlib.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)