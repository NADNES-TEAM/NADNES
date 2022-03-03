cmake_minimum_required(VERSION 3.19)
project(NADNES)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(include/mappers)

add_executable(PPU_test src/main.cpp src/PPU.cpp src/cartridge.cpp src/mappers/NROM.cpp)

