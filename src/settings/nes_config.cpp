#include "settings/nes_config.h"

Config::Config(){
po::options_description server_options("Server configuration");
server_options.add_options()
    ("server.send_rate_hz",po::value<int>(&server_send_rate))
    ("server.ip",po::value<std::string>(&server_ip))
    ("server.port",po::value<int>(&server_port));

po::options_description client_options("Client configuration");
client_options.add_options()
    ("client.force_update_interval_ms", po::value<int>(&client_update_interval))
    ("client.port",po::value<int>(&client_port));

po::options_description emulation_options("Emulation options");
emulation_options.add_options()
    ("emulator.render_rate_hz", po::value<int>(&emulator_render_rate))
    ("emulator.main_clock_rate_hz", po::value<int>(&emulator_clock_rate));

po::options_description ui_paths_options("UI paths");
ui_paths_options.add_options()
    ("ui_paths.apply_cheat_path", po::value<std::string>(&ui_path_apply_cheat))
    ("ui_paths.cheat_window_path", po::value<std::string>(&ui_path_cheat_window))
    ("ui_paths.search_cheat_path", po::value<std::string>(&ui_path_search_cheat))
    ("ui_paths.save_cheat_path", po::value<std::string>(&ui_path_save_cheat))
    ("ui_paths.connect_window_path", po::value<std::string>(&ui_path_connect_window))
    ("ui_paths.keymap_window_path", po::value<std::string>(&ui_path_keymap_window))
    ("ui_paths.player_select_path", po::value<std::string>(&ui_path_player_select));

po::options_description cheat_options("Cheat options");
cheat_options.add_options()
    ("cheats.db_path", po::value<std::string>(&cheats_db_path))
    ("cheats.rows_count", po::value<int>(&cheats_rows_count));

po::options_description all_options("Allowed options");
all_options.add_options()("saves.saves_dir", po::value<std::string>(&saves_dir));
all_options.add(server_options).add(client_options).add(emulation_options);
all_options.add(ui_paths_options).add(cheat_options);

po::variables_map conf_map;
auto file = std::ofstream(CONFIG_FILE, std::ios_base::app);
try {
    po::store(po::parse_config_file(CONFIG_FILE.c_str(), all_options), conf_map);
    po::notify(conf_map);
} catch (boost::program_options::error &e) { error_str = e.what(); }
}

const Config &Config::get_value() {
    static Config config;
    return config;
};
