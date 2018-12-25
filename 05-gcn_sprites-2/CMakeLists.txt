cmake_minimum_required(VERSION 3.13)
project(sdl_premiere_application C)

set(CMAKE_C_STANDARD 11)

find_package(SDL 1 REQUIRED)

add_executable(sprites4 sprites4.c common.h common.c sprite.h sprite.c)
add_executable(sprites5 sprites5.c common.h common.c sprite.h sprite.c)
add_executable(sprites6 sprites6.c common.h common.c sprite.h sprite.c)

target_link_libraries(sprites4 SDL)
target_link_libraries(sprites5 SDL)
target_link_libraries(sprites6 SDL)
