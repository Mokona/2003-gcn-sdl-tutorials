cmake_minimum_required(VERSION 3.13)
project(sdl_premiere_application C)

set(CMAKE_C_STANDARD 11)

find_package(SDL 1 REQUIRED)

add_executable(gcn-fade-1 fade1.c)
add_executable(gcn-fade-2 fade2.c)

target_link_libraries(gcn-fade-1 SDL)
target_link_libraries(gcn-fade-2 SDL)
