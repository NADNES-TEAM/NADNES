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

add_executable( NADNES
        src/ControllerExceptions.cpp
        include/MainWindow.h
        src/MainWindow.cpp
        include/nes.h
        src/nes.cpp
        src/main.cpp
        src/cartridge.cpp src/PPU.cpp
        src/mappers/NROM.cpp
        src/bus.cpp
        src/Controller.cpp
        src/R6502.cpp)

target_link_libraries(NADNES
        Qt::Core
        Qt::Gui
        Qt::Widgets)