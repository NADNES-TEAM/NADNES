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
        src/ControllerExceptions.cpp
        include/MainWindow.h
        src/MainWindow.cpp
        include/nes.h
        src/nes.cpp
        tests/ppu_screen_test.cpp
        src/cartridge.cpp src/PPU.cpp
        src/mappers/NROM.cpp
        src/bus.cpp
        src/Controller.cpp
        src/R6502.cpp)

target_link_libraries(NADNES
        Qt::Core
        Qt::Gui
        Qt::Widgets)

add_executable(Controller_test
        src/main.cpp
        include/Controller.h
        src/Controller.cpp
        include/ControllerExceptions.h
        src/ControllerExceptions.cpp
        tests/Test.h
        tests/Test.cpp
        include/MainWindow.h
        src/MainWindow.cpp
        include/ScreenInterface.h
        include/KeyboardInterface.h)

target_link_libraries(Controller_test
        Qt::Core
        Qt::Gui
        Qt::Widgets)
