#pragma once

#include <boost/program_options.hpp>
#include <string>
#include <ios>
#include <fstream>

const char *const CONFIG_FILE = "../nadnes.conf";

namespace {
namespace po = boost::program_options;

po::variables_map read_config() {
    po::options_description server_options("Server configuration");
    server_options.add_options()
        ("server.send_framerate", po::value<int>())
        ("server.ip",po::value<std::string>())
        ("server.port", po::value<int>());

    po::options_description client_options("Client configuration");
    client_options.add_options()
        ("client.force_update_interval_ms",po::value<int>())
        ("client.port", po::value<int>());

    po::options_description all_options("Allowed options");
    all_options.add(server_options).add(client_options);

    po::variables_map conf_map;
    auto file = std::ofstream(CONFIG_FILE, std::ios_base::app);
    po::store(po::parse_config_file(CONFIG_FILE, all_options), conf_map);
    po::notify(conf_map);
    return conf_map;
};

po::variables_map& get_config() {
    static auto config = read_config();
    return config;
};

}  // namespace

namespace config {
template <typename T>
T get_value(const std::string &key, const T &default_value) {
    auto config = get_config();
    if (config.count(key)) {
        return config[key].as<T>();
    }
    return default_value;
}

template <typename T>
T get_value(const std::string &key) {
    auto config = get_config();
    assert(config.count(key));
    return config[key].as<T>();
}

}