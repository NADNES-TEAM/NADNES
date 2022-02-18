cmake_minimum_required(VERSION 3.22)
project(NADNES)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

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

