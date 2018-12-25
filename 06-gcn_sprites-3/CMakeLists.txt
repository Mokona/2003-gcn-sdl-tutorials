cmake_minimum_required(VERSION 3.13)
project(sdl_premiere_application C)

set(CMAKE_C_STANDARD 11)

find_package(SDL 1 REQUIRED)
find_package(SDL_image 1 REQUIRED)

add_executable(sprites7 sprites7.c ../07-gcn_sprites-4/common.h ../07-gcn_sprites-4/common.c)
add_executable(sprites8 sprites8.c ../07-gcn_sprites-4/common.h ../07-gcn_sprites-4/common.c)
add_executable(sprites9 sprites9.c ../07-gcn_sprites-4/common.h ../07-gcn_sprites-4/common.c)

target_link_libraries(sprites7 SDL SDL_image)
target_link_libraries(sprites8 SDL SDL_image)
target_link_libraries(sprites9 SDL SDL_image)
