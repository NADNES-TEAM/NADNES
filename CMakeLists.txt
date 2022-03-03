cmake_minimum_required(VERSION 3.19)
project(NADNES)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(include/mappers)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(NADNES
        src/main.cpp
        src/R6502.cpp
        src/screen.cpp include/screen_options.h)

target_link_libraries(NADNES
        Qt::Core
        Qt::Gui
        Qt::Widgets)

add_executable(PPU_test
        include/nes.h
        src/nes.cpp
        tests/ppu_screen_test.cpp
        src/cartridge.cpp src/PPU.cpp
        src/mappers/NROM.cpp
        src/screen.cpp
        src/bus.cpp
        src/R6502.cpp)

target_link_libraries(PPU_test
        Qt::Core
        Qt::Gui
        Qt::Widgets)


