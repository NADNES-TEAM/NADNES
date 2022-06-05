#pragma once

#include <boost/program_options.hpp>
#include <fstream>
#include <string>

const std::string CONFIG_FILE = "../nadnes.conf";

namespace {
namespace po = boost::program_options;
}  // namespace

struct Defaults {
    int server_send_rate{20};
    std::string server_ip{"0.0.0.0"};
    int server_port{0};

    int client_update_interval{4};
    int client_port{0};

    int emulator_render_rate{60};
    int emulator_clock_rate{60};

    std::string ui_path_apply_cheat{"../UI/cheating/apply_cheat.ui"};
    std::string ui_path_cheat_window{"../UI/cheating/cheat_window.ui"};
    std::string ui_path_search_cheat{"../UI/cheating/search_cheat.ui"};
    std::string ui_path_save_cheat{"../UI/cheating/save_cheat_window.ui"};
    std::string ui_path_connect_window{"../UI/connect_window.ui"};
    std::string ui_path_keymap_window{"../UI/keymap.ui"};
    std::string ui_path_player_select{"../UI/player_select.ui"};

    std::string cheats_db_path{"../cheats.dat"};
    int cheats_rows_count{500};

    std::string saves_dir{};
};

struct Config : Defaults {
    static const Config& get_value();

    Config(const Config &other) = delete;
    Config(Config &&other) = delete;
    Config operator=(const Config &other) = delete;
    Config operator=(Config &&other) = delete;
    ~Config() = default;

    std::string error_str{};
private:

    Config();
};
