cmake_minimum_required(VERSION 2.8)

set(CMAKE_C_FLAGS_DEBUG "-g3 -O0 -pg")

add_subdirectory(func)
add_executable(Main main.c)
target_link_libraries(Main Func)
