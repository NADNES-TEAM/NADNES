cmake_minimum_required(VERSION 3.14)
project(NADNES)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(include/mappers)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        UiTools
        REQUIRED)

add_executable( NADNES
        include/environment/keymap_window.h
        src/environment/keymap_window.cpp
        include/environment/main_window.h
        src/environment/main_window.cpp
        include/nes.h
        src/nes.cpp
        src/main.cpp
        src/nes_devices/cartridge.cpp src/nes_devices/ppu.cpp
        src/mappers/NROM.cpp
        src/nes_devices/bus.cpp
        src/nes_devices/controller.cpp
        src/nes_devices/R6502.cpp
        src/nes_devices/dma.cpp)

target_link_libraries(NADNES
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::UiTools)
