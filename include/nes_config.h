#pragma once

#include <boost/program_options.hpp>
#include <string>
#include <fstream>
#include <string>
const std::string CONFIG_FILE = "../nadnes.conf";

namespace Config_ {
namespace po = boost::program_options;

po::variables_map read_config();

po::variables_map& get_config();

}  // namespace

namespace defaults {

constexpr int server_send_rate = 20;
const std::string server_ip = "0.0.0.0";
constexpr int server_port = 0;

constexpr int client_update_interval = 4;
constexpr int client_port = 0;

constexpr int emulator_render_rate = 60;

const std::string ui_path_apply_cheat = "../UI/cheating/apply_cheat.ui";
const std::string ui_path_cheat_window = "../UI/cheating/cheat_window.ui";
const std::string ui_path_search_cheat = "../UI/cheating/search_cheat.ui";
const std::string ui_path_save_cheat = "../UI/cheating/save_cheat_window.ui";
const std::string ui_path_connect_window = "../UI/connect_window.ui";
const std::string ui_path_keymap_window = "../UI/keymap.ui";
const std::string ui_path_player_select = "../UI/player_select.ui";

const std::string cheats_db_path = "../cheats.dat";
constexpr int cheats_rows_count = 500;

}

namespace config {

template <typename T>
T get_value(const std::string &key, const T &default_value) {
    auto config = Config_::get_config();
    if (config.count(key)) {
        return config[key].as<T>();
    }
    return default_value;
}

template <typename T>
T get_value(const std::string &key) {
    auto config = Config_::get_config();
    assert(config.count(key));
    return config[key].as<T>();
}

}
