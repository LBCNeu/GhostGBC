cmake_minimum_required(VERSION 3.16)
project(GhostGBC)

find_package(SDL2 REQUIRED)

add_executable(GhostGBC
    core/cpu.cpp
    core/memory.cpp
    core/ppu.cpp
    core/gbc.cpp
    platform/pc/main_sdl.cpp
)

target_link_libraries(GhostGBC PRIVATE SDL2::SDL2 SDL2::SDL2main)
target_include_directories(GhostGBC PRIVATE core/)