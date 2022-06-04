#include "nes_config.h"

namespace Config_ {

po::variables_map read_config(){
po::options_description server_options("Server configuration");
server_options.add_options()
    ("server.send_rate_hz",po::value<int>())
    ("server.ip",po::value<std::string>())
    ("server.port",po::value<int>());

po::options_description client_options("Client configuration");
client_options.add_options()
    ("client.force_update_interval_ms", po::value<int>())
    ("client.port",po::value<int>());

po::options_description emulation_options("Emulation options");
emulation_options.add_options()
    ("emulator.render_rate_hz", po::value<int>())
    ("emulator.main_clock_rate_hz", po::value<int>());

po::options_description ui_paths_options("UI paths");
ui_paths_options.add_options()
    ("ui_paths.apply_cheat_path", po::value<std::string>())
    ("ui_paths.cheat_window_path", po::value<std::string>())
    ("ui_paths.search_cheat_path", po::value<std::string>())
    ("ui_paths.save_cheat_path", po::value<std::string>())
    ("ui_paths.connect_window_path", po::value<std::string>())
    ("ui_paths.keymap_window_path", po::value<std::string>())
    ("ui_paths.player_select_path", po::value<std::string>());

po::options_description cheat_options("Cheat options");
cheat_options.add_options()
    ("cheats.db_path", po::value<std::string>())
    ("cheats.rows_count", po::value<int>());

po::options_description all_options("Allowed options");
all_options.add(server_options).add(client_options).add(emulation_options);
all_options.add(ui_paths_options).add(cheat_options);

po::variables_map conf_map;
auto file = std::ofstream(CONFIG_FILE, std::ios_base::app);
try {
    po::store(po::parse_config_file(CONFIG_FILE.c_str(), all_options), conf_map);
} catch (boost::program_options::error &e){

}
po::notify(conf_map);
return conf_map;
}

po::variables_map& get_config() {
    static auto config = read_config();
    return config;
};

}
