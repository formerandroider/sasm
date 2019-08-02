add_executable(sasm_run sasm.c)
target_include_directories(sasm_run PRIVATE ../include)
target_link_libraries(sasm_run sasm)