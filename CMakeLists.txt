cmake_minimum_required(VERSION 3.19)
project(NADNES)

# needed for qt
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

# Don't remove include/Emulator.h and other classes derived from QT
add_executable(NADNES
        include/Emulator.h
        src/Emulator.cpp
        src/main.cpp
        src/screen.cpp)

target_link_libraries(NADNES
        Qt::Core
        Qt::Gui
        Qt::Widgets)

