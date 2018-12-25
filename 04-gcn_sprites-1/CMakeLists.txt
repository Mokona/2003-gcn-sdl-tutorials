cmake_minimum_required(VERSION 3.13)
project(sdl_premiere_application C)

set(CMAKE_C_STANDARD 11)

find_package(SDL 1 REQUIRED)

add_executable(sprites1 sprites1.c common.h common.c)
add_executable(sprites2 sprites2.c common.h common.c)
add_executable(sprites3 sprites3.c common.h common.c)

target_link_libraries(sprites1 SDL)
target_link_libraries(sprites2 SDL)
target_link_libraries(sprites3 SDL)
