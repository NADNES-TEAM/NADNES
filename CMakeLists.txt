cmake_minimum_required(VERSION 3.15)
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
#find_package(SQLite3)
find_package(Boost COMPONENTS program_options REQUIRED)

if (APPLE)
    set(MACOSX_BUNDLE_ICON_FILE nadnes.icns)
    set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/icons/nadnes.icns")
    set_source_files_properties(${app_icon_macos} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
    set(common_ui_files
            "UI/connect_window.ui"
            "UI/keymap.ui"
            "UI/player_select.ui")

    set(cheat_ui_files
            "UI/cheating/apply_cheat.ui"
            "UI/cheating/search_cheat.ui"
            "UI/cheating/save_cheat_window.ui"
            "UI/cheating/cheat_window.ui")
    set_source_files_properties(${common_ui_files} PROPERTIES MACOSX_PACKAGE_LOCATION "UI")
    set_source_files_properties(${cheat_ui_files} PROPERTIES MACOSX_PACKAGE_LOCATION "UI/cheating")
endif ()


if(APPLE)
    add_executable(NADNES MACOSX_BUNDLE ${app_icon_macos} ${common_ui_files} ${cheat_ui_files})
else()
    add_executable(NADNES)
endif()

target_sources(NADNES PUBLIC src/environment/connection_window.cpp
        include/environment/connection_window.h
        include/remote_emulator.h
        include/remote_player.h
        src/gamepad.cpp
        include/gamepad.h
        include/environment/player_manager_window.h
        src/environment/player_manager_window.cpp
        include/main_controller.h
        include/environment/keymap_window.h
        src/environment/keymap_window.cpp
        include/environment/main_window.h
        src/environment/main_window.cpp
        include/environment/cheating/cheat_window.h
        src/environment/cheating/cheat_window.cpp
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
        src/remote_player.cpp
        include/colors_map.h
        src/colors_map.cpp
        include/server.h
        src/server.cpp
        include/environment/cheating/cheat_fwd.h
        src/cheat_db_handler_files.cpp
        include/environment/cheating/apply_cheat.h
        include/environment/cheating/search_cheat.h
        src/environment/cheating/apply_cheat.cpp
        src/environment/cheating/search_cheat.cpp
        src/nes_config.cpp
        src/nes_utils.cpp)

#if (SQLITE3_FOUND)
#    message("Found sqlite3!")
#    include_directories(${SQLite3_INCLUDE_DIR})
#    target_link_libraries (NADNES ${SQLite3_LIBRARY})
#endif()

target_link_libraries(NADNES
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::UiTools
        Qt::Network
        Boost::program_options)
