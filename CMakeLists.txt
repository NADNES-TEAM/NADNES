cmake_minimum_required(VERSION 3.15)
project(NADNES)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(include/mappers)
include_directories(uis)

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        UiTools
        Network
        REQUIRED)
find_package(SQLite3)

add_executable(NADNES
        src/environment/connection_window.cpp
        include/environment/connection_window.h
        include/remote_emulator.h
        include/remote_player.h
        src/gamepad.cpp
        include/gamepad.h
        include/environment/player_select_window.h
        src/environment/player_select_window.cpp
        include/main_controller.h
        include/environment/keymap_window.h
        src/environment/keymap_window.cpp
        include/environment/main_window.h
        src/environment/main_window.cpp
        include/environment/cheat_window.h
        src/environment/cheat_window.cpp
        include/nes.h
        src/nes.cpp
        src/main.cpp
        src/nes_devices/cartridge.cpp
        src/nes_devices/ppu.cpp
        src/mappers/NROM.cpp
        src/mappers/UNROM.cpp
        src/nes_devices/bus.cpp
        src/nes_devices/controller.cpp
        src/main_controller.cpp
        src/nes_devices/R6502.cpp
        include/search.h
        src/nes_devices/search.cpp
        src/nes_devices/dma.cpp
        src/local_emulator.cpp
        include/local_emulator.h
        src/remote_emulator.cpp
        include/remote_player.h
        src/remote_player.cpp
        include/colors_map.h
        src/colors_map.cpp
        include/Server.h
        src/Server.cpp
        src/environment/cheat_window.cpp
        include/environment/search_cheat.h
        src/environment/search_cheat.cpp
        include/environment/apply_cheat.h
        src/environment/apply_cheat.cpp
        include/cheating/cheat_format.h
        src/cheating/cheat_format.cpp
        src/nes_devices/search.cpp
#        include/cheat_db_handler.h src/cheat_db_handler.cpp
        include/environment/cheating/cheat_fwd.h
        include/cheat_db_handler2.h src/cheat_db_handler2.cpp)

if (SQLITE3_FOUND)
    message("Found sqlite3!")
    include_directories(${SQLite3_INCLUDE_DIR})
    target_link_libraries (NADNES ${SQLite3_LIBRARY})
endif()

target_link_libraries(NADNES
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::UiTools
        Qt::Network)
