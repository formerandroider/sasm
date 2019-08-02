add_executable(sasm sasm.c)
target_include_directories(sasm PRIVATE ../include)
target_link_libraries(sasm libsasm)