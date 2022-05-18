#pragma once
#include<vector>
#include<cstdint>
namespace Search {
enum class Action { save, encrease, decrease, encrease_or_save, changed, decrease_or_save, all ,eq_num,g_num,geq_num,l_num,leq_num};
enum class Input { raw_bytes, num };
enum class Place { RAM, ROM };
struct ResultRaw {
    std::size_t address;
    std::vector<uint8_t> old_data, cur_data;
};
struct Params {
    Input type;
    Place place;
    std::vector<uint8_t> data_in;
    [[nodiscard]] long long convert_to_num(const std::vector<uint8_t> &elem) const;
};
struct ParamsOfSearch : Params {
    bool is_initial{true};
    Action event;
    bool check_coincidence(ResultRaw &raw) const;
};
struct ParamsOfChange : Params {
    std::size_t index;
};

}
