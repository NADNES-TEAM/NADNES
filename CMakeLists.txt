cmake_minimum_required(VERSION 3.15)
project(NADNES)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(include/nes_core/mappers)
include_directories(include/nes_core)

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

target_sources(NADNES PUBLIC src/widgets/connection_window.cpp
        include/widgets/connection_window.h
        include/remote_emulator.h
        include/network/remote_player.h
        src/gamepad.cpp
        include/gamepad.h
        include/widgets/player_manager_window.h
        src/widgets/player_manager_window.cpp
        include/main_controller.h
        include/widgets/keymap_window.h
        src/widgets/keymap_window.cpp
        include/widgets/main_window.h
        src/widgets/main_window.cpp
        include/cheating/cheat_window.h
        src/cheating/cheat_window.cpp
        include/nes_core/nes.h
        src/nes_core/nes.cpp
        src/main.cpp
        src/nes_core/nes_devices/cartridge.cpp
        src/nes_core/nes_devices/ppu.cpp
        src/nes_core/mappers/NROM.cpp
        src/nes_core/mappers/UNROM.cpp
        src/nes_core/nes_devices/bus.cpp
        src/nes_core/nes_devices/controller.cpp
        src/main_controller.cpp
        src/nes_core/nes_devices/R6502.cpp
        include/cheating/search.h
        src/nes_core/nes_devices/search.cpp
        src/nes_core/nes_devices/dma.cpp
        src/local_emulator.cpp
        include/local_emulator.h
        src/remote_emulator.cpp
        src/network/remote_player.cpp
        include/settings/colors_map.h
        src/settings/colors_map.cpp
        include/network/server.h
        src/network/server.cpp
        include/cheating/cheat_fwd.h
        src/cheating/cheat_db_handler_files.cpp
        include/cheating/apply_cheat.h
        include/cheating/search_cheat.h
        src/cheating/apply_cheat.cpp
        src/cheating/search_cheat.cpp
        src/settings/nes_config.cpp
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
