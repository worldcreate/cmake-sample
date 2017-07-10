cmake_minimum_required(VERSION 2.8)
add_subdirectory(func)
add_executable(Main main.c)
target_link_libraries(Main Func)
