cmake_minimum_required(VERSION 3.12)
project(Breakout C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 11)

add_executable(Breakout
    src/main.c
    src/init.c
)

find_package(SDL2 REQUIRED)

target_include_directories(Breakout PRIVATE include)
target_include_directories(Breakout PRIVATE ${SDL2_INCLUDE_DIRS})

target_link_libraries(Breakout SDL2 SDL2_image SDL2_ttf)