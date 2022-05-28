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
        Network
        REQUIRED)

add_executable( NADNES
        src/gamepad.cpp
        include/gamepad.h
        include/environment/player_select_window.h
        src/environment/player_select_window.cpp
        include/main_controller.h
        include/environment/keymap_window.h
        src/environment/keymap_window.cpp
        include/environment/main_window.h
        src/environment/main_window.cpp
        include/nes.h
        src/nes.cpp
        src/main.cpp
        src/nes_devices/cartridge.cpp src/nes_devices/ppu.cpp
        src/mappers/NROM.cpp
        src/mappers/UNROM.cpp
        src/nes_devices/bus.cpp
        src/nes_devices/controller.cpp
        src/main_controller.cpp
        src/nes_devices/R6502.cpp
        src/nes_devices/dma.cpp
        src/local_emulator.cpp
        include/local_emulator.h src/remote_emulator.cpp include/remote_player.h src/remote_player.cpp include/colors_map.h src/colors_map.cpp include/Server.h src/Server.cpp)

target_link_libraries(NADNES
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::UiTools
        Qt::Network)
