cmake_minimum_required(VERSION 3.13)
project(sdl_premiere_application C)

set(CMAKE_C_STANDARD 11)

find_package(SDL 1 REQUIRED)

add_executable(gcn-inputs input.c)

target_link_libraries(gcn-inputs SDL)
