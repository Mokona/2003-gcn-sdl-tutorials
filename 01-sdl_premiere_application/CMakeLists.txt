cmake_minimum_required(VERSION 3.13)
project(sdl_premiere_application C)

set(CMAKE_C_STANDARD 11)

find_package(SDL 1 REQUIRED)

add_executable(sdl_premiere_application premier_prog_sdl.c)
target_link_libraries(sdl_premiere_application SDL)