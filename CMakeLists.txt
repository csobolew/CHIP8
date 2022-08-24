cmake_minimum_required(VERSION 3.23)
project(CHIP8)

set(CMAKE_CXX_STANDARD 14)

set(SDL2_INCLUDE_DIR C:/libs/SDL/include)
set(SDL2_LIB_DIR C:/libs/SDL/lib/x64)
include_directories(${SDL2_INCLUDE_DIR})
link_directories(${SDL2_LIB_DIR})
include_directories(.)


add_executable(CHIP8 main.cpp chip8.cpp)
target_link_libraries(${PROJECT_NAME} SDL2main SDL2)
